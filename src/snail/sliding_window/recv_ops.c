#include "sliding_window.h"


static inline int
_recv_pkg (struct pkg *pkg, bool ack)
{
  errno = 0;

  int ret = 0;
    
  memset (pkg, 0, sizeof *pkg);

  printf("esperando por %d\n", sliding_window.expected_pkg_num);
  while (1)
    {
      ret = recv_pkg(pkg); 
      if (ret != EXIT_SUCCESS)
        {
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

int
snail_recv (struct pkg *pkg)
{
  return _recv_pkg(pkg, false);
}

int 
snail_recv_and_ack (struct pkg *pkg)
{
  return _recv_pkg(pkg, true);
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
        if (free_pkg_n (sliding_window.res.sequence_number) == EXIT_SUCCESS)
          return EXIT_SUCCESS;
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
  }
}
