/*
 * unix stealth demo ELF loader either from disk or network
 * various techniques involving,
 * - cmdline and ppid spoofing, orphan to init process
 * - process hollowing for bypassing application whitelisting
 * - modify /proc/self/exe via ptrace to point toward valid executable
 * - memfd_secret and fexecve for optional fileless exec & no-tmpfs hidden vma
 *
*/
