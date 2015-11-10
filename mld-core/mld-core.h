#ifndef _MLD_CORE_H_
#define _MLD_CORE_H_

void mld_register_alloc(void *ptr, size_t size);
bool mld_deregister_alloc(void *ptr);
size_t mld_get_number_of_leaks();
void mld_dump_leaks();

#endif
