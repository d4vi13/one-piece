#include "sliding_window.h"

struct sliding_window sliding_window;

void 
init_sliding_window ()
{
  sliding_window.head = 0;
  sliding_window.seq_num = 0;
}

uint8_t
get_seq_num ()
{
  return sliding_window.seq_num++;
}

int 
window_full ()
{
  return sliding_window.counter >= WINDOW_SIZE ;
}

uint8_t
pkg_n_pos (uint8_t n)
{
  uint8_t pos, offset;
  offset = n - sliding_window.pkgs[sliding_window.head].sequence_number;
  pos = (sliding_window.head + offset) % WINDOW_SIZE;

  return pos;
}


int free_pkg_n(uint8_t n)
{

  if (sliding_window.counter == 0)
    return EXIT_FAILURE;

  int i = 0;
  while (i < sliding_window.counter)
  {
    uint8_t pos = (sliding_window.head + i) % WINDOW_SIZE;
    if (sliding_window.pkgs[pos].sequence_number == n)
    {
      // Avança o head até liberar o pacote n
      int to_free = i + 1;
      sliding_window.head = (sliding_window.head + to_free) % WINDOW_SIZE;
      sliding_window.counter -= to_free;
      return EXIT_SUCCESS;
    }
    i++;
  }

  // Pacote n não encontrado
  return EXIT_FAILURE;
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

static inline void
print_sw_status ()
{
  printf ("head: %d\n", sliding_window.head);
  printf ("counter: %d\n", sliding_window.counter);
  printf ("expected_pkg_num: %d\n", sliding_window.expected_pkg_num);

  printf("%d %d %d %d\n", sliding_window.pkgs[0].sequence_number, sliding_window.pkgs[1].sequence_number, sliding_window.pkgs[2].sequence_number, sliding_window.pkgs[3].sequence_number);
}
