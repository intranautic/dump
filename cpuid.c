#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// enforce ordering, platform endianess may fuck with array
// explicit fields much more programmatic and verbose
typedef struct cpuinfo cpuinfo_t;
struct cpuinfo {
  int cpuid;
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
};

static void cpuinfo_dump(cpuinfo_t* cinfo) {
  printf(
    "cpuinfo {\n\tcpuid: %d\n\teax: %#x\n\tebx: %#x\n\tecx: %#x\n\tedx: %#x\n}\n",
    cinfo->cpuid,
    cinfo->eax,
    cinfo->ebx,
    cinfo->ecx,
    cinfo->edx
  );
  return;
}

// gcc has no __cpuid intrinsic unlike msvc?
static int __attribute__((noinline)) cpuidx86(cpuinfo_t* cinfo) {
  if (!cinfo)
    return -1;

  asm volatile (
    ".intel_syntax noprefix;"
    "mov eax, %[id];"
    "cpuid;"
    "mov %0, eax;"
    "mov %1, ebx;"
    "mov %2, ecx;"
    "mov %3, edx;"
    ".att_syntax;"
      : "=r" (cinfo->eax),
        "=r" (cinfo->ebx),
        "=r" (cinfo->ecx),
        "=r" (cinfo->edx)
      : [id] "r" (cinfo->cpuid)
      : "eax"
  );
  return 0;
}

int main(int argc, char** argv) {
  cpuinfo_t cinfo0 = {.cpuid=0}, cinfo1 = {.cpuid=1};
  if (cpuidx86(&cinfo0) || cpuidx86(&cinfo1))
    return puts("Error retrieving cpuid information");

  cpuinfo_dump(&cinfo0);
  cpuinfo_dump(&cinfo1);
  // check hypervisor feature bit cpuid(1)->ecx.31
  printf("\ncpuid(1) -> ecx.hypervisor = %d\n", (cinfo1.ecx>>31) & 1);

  // build vendor string
  char vendor[13] = {0};
  *(uint32_t *)vendor = cinfo0.ebx;
  *(uint32_t *)(vendor + 4) = cinfo0.edx;
  *(uint32_t *)(vendor + 8) = cinfo0.ecx;
  printf("vendor string: %s\n", vendor);
  return 0;
}
