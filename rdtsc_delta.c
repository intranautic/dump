#include <stdio.h>
#include <x86intrin.h>

// calculate rdtsc delta due to cpuid forcing vmexit
// multiple factor k increases delta making difference larger
static size_t rdtsc_delta(int k) {
  _mm_lfence();
  size_t timer = __rdtsc();
  for (int i=0; i<k; ++i)
    asm volatile (
      ".intel_syntax noprefix;"
      "mov eax, 0;"
      "cpuid;"
      ".att_syntax;"
        :
        :
        : "eax"
    );
  return __rdtsc() - timer;
}

int main(int argc, char** argv) {
  if (argc < 2)
    return printf("Usage: %s <vm_threshold> <k_factor>\n", argv[0]);

  //default: vm_threshold=750, k_factor=5
  //this seems to work best, can play around more
  int vm_threshold = atoi(argv[1]), k_factor = atoi(argv[2]);
  int delta = rdtsc_delta(k_factor);
  printf("vm_threshold: %d\nk_factor: %d\nrdtsc_delta: %d\nvm detect: %s\n",
    vm_threshold*k_factor,
    k_factor,
    delta,
    (delta > vm_threshold*k_factor) ? "yes" : "no");
  return 0;
}
