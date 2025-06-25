#include "sliding_window.h"

struct sliding_window sliding_window;

static inline void
print_sw_status ()
{
  printf ("head: %d\n", sliding_window.head);
  printf ("counter: %d\n", sliding_window.counter);
  printf ("expected_pkg_num: %d\n", sliding_window.expected_pkg_num);

  for (int i = 0; i < WINDOW_SIZE; i++)
  {
    if (i == sliding_window.head)
      printf ("(%d:%d) ", sliding_window.pkgs[i].sequence_number, sliding_window.pkgs[i].type);
    else
      printf ("%d:%d ", sliding_window.pkgs[i].sequence_number, sliding_window.pkgs[i].type);
  }
  sleep(1);
}


void 
init_sliding_window ()
{
  sliding_window.head = 0;
}

static inline int 
window_full ()
{
  return sliding_window.counter >= WINDOW_SIZE ;
}

static inline uint8_t
pkg_n_pos (uint8_t n)
{
  uint8_t pos, offset;
  offset = n - sliding_window.pkgs[sliding_window.head].sequence_number;
  pos = (sliding_window.head + offset) % WINDOW_SIZE;

  return pos;
}

int 
free_pkg_n (uint8_t n)
{
  uint8_t i, pos = pkg_n_pos (n);
  i = 0;


  while ((((sliding_window.head + i)  % WINDOW_SIZE) != pos) ) i++;


  sliding_window.head = (pos + 1) % WINDOW_SIZE;
  sliding_window.counter = sliding_window.counter - i - 1; 

  return EXIT_SUCCESS;
}

int 
resend (uint8_t n)
{
  errno = 0;

  int ret, i = sliding_window.head;

  do
    {
      ret = 0;
      ret = send_pkg(&sliding_window.pkgs[i]);
      if (ret == EXIT_FAILURE)
        {
          continue;
        }
      i = (i + 1) % WINDOW_SIZE;
    }
  while (i != sliding_window.head);

  return EXIT_SUCCESS;
}

int
wait_any_res ()
{
  errno = 0;
  int ret = 0;

  while (1)
  {
    ret = recv_pkg (&sliding_window.res);
    if (ret == EXIT_FAILURE)
      {
        if (ERRNO_IS_TIMEOUT)
          {
            perror ("Time out, reenviar toda a janela");
            resend (sliding_window.head);
          }
        errno = 0;
        continue;
      }

    if (ACKED(sliding_window.res.type) && IN_RANGE(sliding_window.res.sequence_number))
      {
        free_pkg_n (sliding_window.res.sequence_number);
        return EXIT_SUCCESS;
      }

    if (sliding_window.res.type == NACK)
      {
        uint8_t pos = pkg_n_pos (sliding_window.res.sequence_number);
        resend (pos);
      }
  }
}

struct pkg *
wait_pkg_n (uint8_t n)
{
  errno = 0;
  
  int ret = 0;

  while (1)
  {
    ret = recv_pkg (&sliding_window.res);
    if (ret == EXIT_FAILURE)
      {
        if (ERRNO_IS_TIMEOUT)
          {
            perror ("Time out, reenviar toda a janela: ");
            resend (sliding_window.head);
          }
        continue;
      }

    // TODO make a macro that check all possible ACK types
    if (ACKED(sliding_window.res.type))
      {
        if (sliding_window.res.sequence_number >= n){
          free_pkg_n (sliding_window.res.sequence_number);
          return &sliding_window.res;
        }
      }

    if (sliding_window.res.type == NACK)
      {
        uint8_t pos = pkg_n_pos (sliding_window.res.sequence_number);
        resend (pos);
      }
  }
}

void 
add_pkg_to_window (struct pkg *pkg)
{
  static int init = 0; // ensures first pkg goes on the first slot

  uint8_t idx;
  if (init) 
    idx = (sliding_window.head + sliding_window.counter) % WINDOW_SIZE;
  else
    {
      idx = 0;
      init++;
    }

  memcpy (&sliding_window.pkgs[idx], pkg, sizeof*pkg);
  sliding_window.counter++;
}

int 
snail_send (struct pkg *pkg)
{
  errno = 0;

  if (window_full ())
    {
      wait_any_res ();
    }

  
  /* adiciona pacote na janela */
  add_pkg_to_window (pkg);

  /* manda pacote */
  while (send_pkg(pkg) == EXIT_FAILURE);

  return EXIT_SUCCESS;
}

int
snail_recv (struct pkg *pkg, int ack)
{
  errno = 0;

  int ret = 0;
    
  memset (pkg, 0, sizeof *pkg);

  while (1)
    {
      ret = recv_pkg(pkg); 
      if (ret != EXIT_SUCCESS)
        {
        //perror ("Nao pode receber pacote: ");
          continue;
        }
      if (pkg->sequence_number == sliding_window.expected_pkg_num && (!ACKED(pkg->type)))
        break;
      else 
        {
          resend_last_ack ();
          continue;
        }
    }
  
  sliding_window.expected_pkg_num++;

  if (ack)
    ack_pkg (pkg->sequence_number);

  return EXIT_SUCCESS;
}

void
send_start_talking ()
{
  errno = 0;
  struct pkg pkg;
  prepare_ack_pkg (&pkg, get_seq_num (), FREE);
  snail_send (&pkg);
    
  wait_pkg_n(pkg.sequence_number); 

}