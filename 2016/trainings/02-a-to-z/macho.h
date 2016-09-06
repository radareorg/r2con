
#define uint32_t unsigned int


typedef enum cpu_type_t {
	CPU_TYPE_X86 = 7,
	CPU_TYPE_X86_64 = 0x1000007,
	CPU_TYPE_ARM = 12,
	CPU_TYPE_POWERPC = 18,
} cpu_type_t;

typedef enum cpu_subtype_t {
	CPU_SUBTYPE_X86_ALL = 3,
	CPU_SUBTYPE_BIG_ENDIAN = 1,
	CPU_SUBTYPE_POWERPC_ALL = 0,
	CPU_SUBTYPE_ANY = 0x3010000
} cpu_subtype_t;

struct mach_header {
	uint32_t magic;
	cpu_type_t cputype;
	cpu_subtype_t cupsubtype;
	uint32_t filetype;
	uint32_t ncmds;
	uint32_t sizeofcmds;
	uint32_t flags;
};
