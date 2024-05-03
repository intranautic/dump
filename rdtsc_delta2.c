// fixed up some shit, idk if this is better but i dont really care
// processor clock frequency normalization is completely fucked, useless
// useless useless
#include <stdio.h>
#include <x86intrin.h>
#include <cpuid.h>

#define K             100
#define TSC_BASELINE  1000

// calculate rdtsc delta due to cpuid forcing vmexit
// multiple factor k increases delta making eval more accurage
static inline size_t rdtsc_delta(void) {
  _mm_lfence();
  size_t timer = __rdtsc();
  for (int i=0; i<K; ++i) {
    asm volatile (
      ".intel_syntax noprefix;"
      "mov eax, 0;"
      "cpuid;"
      ".att_syntax;"
        :
        :
        : "eax"
    );
    _mm_lfence();
  }
  return __rdtsc() - timer;
}

static inline unsigned cpuid_avghz(void) {
  int eax, ebx, _;
  __cpuid(0x16, eax, ebx, _, _);
  return (eax+ebx)/2;
}

int main(int argc, char** argv) {
  // calculate normalized delta by factoring clock speed
  unsigned avghz = cpuid_avghz();
  size_t delta = rdtsc_delta();
  printf("cpu average clock speed (Hz): %u\nprocessor rdtsc delta: %lu (%lf * %d)\n",
    avghz,
    delta,
    (double)delta/K,
    K);

  printf("hypervisor enabled: %s\n",
    (!avghz || delta > TSC_BASELINE*K) ? "yes" : "no");
  return 0;
}
