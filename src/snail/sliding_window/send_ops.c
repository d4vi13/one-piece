#include "sliding_window.h"

static inline struct pkg *
_send_pkg (struct pkg *pkg, bool wait)
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

  if (wait)
    {
        return wait_pkg_n(pkg->sequence_number);
    }
  return pkg;
}

int 
snail_send (struct pkg *pkg)
{
  errno = 0;

  if (_send_pkg (pkg, false) == pkg)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

struct pkg * 
snail_send_and_wait (struct pkg *pkg)
{
    return _send_pkg(pkg, true);
}

static inline int 
_ack_pkg (uint8_t seq_num, pkg_t type)
{
  errno = 0;

  prepare_ack_pkg (&sliding_window.ack, seq_num, type);
  while (send_pkg (&sliding_window.ack) == EXIT_FAILURE);

  return EXIT_SUCCESS;
   
}

int 
ack_pkg (uint8_t seq_num)
{
  return _ack_pkg (seq_num, ACK);
} 

int 
error_pkg (uint8_t seq_num)
{
  return _ack_pkg (seq_num, ERROR);
}

int 
ok_ack_pkg (uint8_t seq_num)
{
  return _ack_pkg (seq_num, OK_ACK);
}

int 
treasure_ack (uint8_t seq_num, pkg_t pkg_type, char *filename)
{
  errno = 0;

  prepare_treasure_pkg (&sliding_window.ack, pkg_type, seq_num, filename);
  while (send_pkg (&sliding_window.ack) == EXIT_FAILURE);

  return EXIT_SUCCESS;
}

int 
resend_last_ack ()
{
  errno = 0;

  while (send_pkg (&sliding_window.ack) == EXIT_FAILURE);

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

