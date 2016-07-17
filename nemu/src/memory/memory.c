#include "common.h"
#include "cpu/reg.h"
#include "device/mmio.h"


uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t l1_cache_read(hwaddr_t , size_t);
void l1_cache_write(hwaddr_t, size_t, uint32_t);
lnaddr_t seg_translate(swaddr_t, size_t, uint8_t );
hwaddr_t page_translate(lnaddr_t );

typedef union
{
	struct
	{
		uint32_t tag : 20;
		uint32_t page : 32;
		uint32_t valid : 1;
	};
	uint32_t val;
}TLB;

TLB tlb[64];

void init_tlb()
{
	int i = 0;
	for(i = 0;i < 64;i++)
	{
		tlb[i].valid = 0;
	}
}

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int mmio = is_mmio(addr);
	if(mmio == -1)
		//return l1_cache_read(addr, len) & (~0u >> ((4 - len) << 3));
		return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	else 
		return mmio_read(addr, len, mmio);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int mmio = is_mmio(addr);
	if(mmio == -1)
		//l1_cache_write(addr, len, data);
		dram_write(addr, len, data);
	else 
		mmio_write(addr, len, data, mmio);		 
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	if(cpu.cr0.protect_enable == 1 && cpu.cr0.paging == 1)
	{
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
	}
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	if(cpu.cr0.protect_enable == 1 && cpu.cr0.paging == 1)
	{
		hwaddr_t hwaddr = page_translate(addr);
		hwaddr_write(hwaddr, len, data);
		return;
	}
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	if(cpu.cr0.protect_enable == 0)
		return lnaddr_read(addr, len);
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	if(cpu.cr0.protect_enable == 0)
	{
		lnaddr_write(addr, len, data);
		return;
	}
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	lnaddr_write(lnaddr, len, data);
}

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg)
{
	//uint32_t base_1 = cpu.gdt[sreg].base_1;	
	//uint32_t base_2 = cpu.gdt[sreg].base_2;
	//uint32_t base_3 = cpu.gdt[sreg].base_3;
	//uint32_t base = (base_1<<16) + (base_2 << 8) + base_3 ;
	//swaddr_t lnaddr = addr + base;
	//printf("%x\n",base);
	//return lnaddr;
	return addr;
}

hwaddr_t page_translate(lnaddr_t lnaddr)
{
//	int i;
	//int flag = 0;
/*	for(i = 0;i<64;i++)
	{
		if(tlb[i].tag == (lnaddr >> 12) && tlb[i].valid == 1)
		{
			flag = 1;
			break;
		}
	}
	//printf("#%d ",flag);
	if(flag == 1) //hit in tlb
	{
		uint32_t offset = lnaddr &0xfff;
		uint32_t phy =  offset + ((tlb[i].page >> 12) << 12);
		return phy;
	}*/
	uint16_t dir = lnaddr >>22;
	uint16_t page = (lnaddr >> 12)&0x3ff;
	uint32_t offset = lnaddr&0xfff;
	uint32_t page_base = hwaddr_read((cpu.cr3.page_directory_base<<12)+dir*4,4)>>12;
	uint32_t phyaddr =  offset+((hwaddr_read((page_base<<12)+page*4,4)>>12)<<12); 
	//printf("%x ",phy);
	//if( page_base == 0)
	//	return offset;
	
/*	int position = 1;
	tlb[position].valid = 1;
	tlb[position].tag = lnaddr >> 12;
	tlb[position].page = hwaddr_read((page_base<<12)+page*4,4);
*/		
	return phyaddr;

}

/*hwaddr_t page_translate(lnaddr_t lnaddr) {
	    hwaddr_t pde_addr = (cpu.cr3.val & 0xFFFFF000) + ((lnaddr >> 20) & 0xFFC);
		    uint32_t pde = hwaddr_read(pde_addr, 4);
			    //vm_assert(pde & 1, "PDE not present, address = %#10x", lnaddr);
				  //  vm_assert(((pde >> 7) & 1) == 0, "4MB page not supported");
					    hwaddr_t pte_addr = (pde & 0xFFFFF000) + ((lnaddr >> 10) & 0xFFC);
						    uint32_t pte = hwaddr_read(pte_addr, 4);
					//		    vm_assert(pte & 1, "PTE not present, address = %#10x", lnaddr);
								    hwaddr_t hwaddr = (pte & 0xFFFFF000) | (lnaddr & 0xFFF);
									    return hwaddr;
}
*/

