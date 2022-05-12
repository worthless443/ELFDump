#include<stdio.h>
#include<string.h>

#include<elff.h>

// TODO organize, clean

struct entry_stuff {
	int x,y,z,h,i;
};

struct callers {
	char* (*f1)(ELF64_ProgramHeaderEntryRef);
	int (*f2)(ELF64_ProgramHeaderEntryRef);
	char *(*f3)(ELF64_ProgramHeaderEntryRef);
	char *(*f4)(ELF64_ProgramHeaderEntryRef);
	int (*f5)(ELF64_ProgramHeaderEntryRef);
};
struct program_header_specs {
	char* Entrytype;	
	int *Attrs;	
	char *AttrsString;	
	char *EntryTypestring;	
	int  *FileSize;
};


void mk_callers(int *arr[]) {
#define size 2 
	arr[0] = (int*)ELF64_ProgramHeaderEntryGetTypeString;
	arr[1] = (int*)ELF64_ProgramHeaderEntryGetAttributes;
}
void create_hdr_specs(ELF64_ProgramHeaderEntryRef pheader, struct  program_header_specs **specs) {
	struct program_header_specs sp;// = malloc(1000);
	printf("nigger");
	int *arr[size];
	mk_callers(arr);
	for(int i=0; i<size;i++)  {
		int name = ((int(*)(ELF64_ProgramHeaderEntryRef))*(arr+i))(pheader);
	}
		
}
ELF64_SectionHeaderEntryRef get_offset(ELF64_FileRef *file, int index) {
	ELF64_Off   offst; 
    	ELF64_Half      entrySize;
	ELF64_HeaderRef header;//= malloc(100);
	if(!ELF64_FileIsValid(*file)) return NULL;
	header = ELF64_FileGetHeader(*file);
	offst = ELF64_HeaderGetSectionHeaderOffset(header);
	entrySize = ELF64_HeaderGetSectionHeaderEntrySize(header);
    	return ( ELF64_SectionHeaderEntryRef )( ( void * )( ( char * )(*file)+ offst + (2<<index)*index)); //( index * entrySize) ) );
}

char* get_offset_dif_type(ELF64_FileRef *file, int index) {
	ELF64_Off   offst; 
    	ELF64_Half      entrySize;
	if(!ELF64_FileIsValid(*file)) return NULL;
	ELF64_SectionHeaderEntryRef header = ELF64_FileGetSectionHeaderEntry(*file, index);

	ELF64_SymbolTableEntryRef sym = ELF64_FileGetSymbolTableEntryForSection(*file, header, 4);

	offst = ELF64_SymbolTableEntryGetNameOffset(sym);
return  ( char * )sym + offst +  index;
}


char* get_offset_h(ELF64_FileRef *file, int index) {
    	ELF64_Word                offst;
    	ELF64_Half      entrySize;
	ELF64_HeaderRef header;//= malloc(100);
	if(!ELF64_FileIsValid(*file)) return NULL;
	header = ELF64_FileGetHeader(*file);

	offst = ELF64_HeaderGetSectionHeaderOffset(header);
	return (char*)get_offset(file,index) - offst;

}

struct entry_stuff *get_entry(ELF64_SectionHeaderEntryRef rice) {
	struct entry_stuff *stuff_ent = malloc(1000);

	int *arr = malloc(sizeof(struct entry_stuff)*100);
	char *nice = (char*)rice;	
	int i=0;
	while((*(int*)(nice))!=0) {
		arr[i] = *(int*)nice;
		nice = nice + 1;
		i+=1;
	}
	stuff_ent  = (struct entry_stuff*)arr;
	return stuff_ent;
}

ELF64_SymbolTableEntryRef  mk_sym(ELF64_FileRef *file, ELF64_SectionHeaderEntryRef section, unsigned int index) {
	ELF64_SymbolTableEntryRef entry =  ELF64_FileGetSymbolTableEntryForSection(*file, section, index);
	entry =  (ELF64_SymbolTableEntryRef)((char*)file+ ELF64_SymbolTableEntryGetNameOffset(entry));
	int *i = malloc(sizeof(int));
	for(*i=(int)ELF64_SymbolTableEntryGetSymbolValue(entry); *i<(int)ELF64_SymbolTableEntryGetObjectSize(entry);*i+=1);
	return entry + (ELF64_SymbolTableEntryGetNameOffset(entry) - (*i));
}

ELF64_ProgramHeaderEntryRef get_program_entry_ref(ELF64_FileRef *file, int index) {
	ELF64_ProgramHeaderEntryRef header = ELF64_FileGetProgramHeaderEntry(*file, index);
	ELF64_Off offset = ELF64_ProgramHeaderEntryGetFileOffset(header);
	return (ELF64_ProgramHeaderEntryRef)((char*)header+  offset +  ((1<<index)*index));
}

int err_return  = 2;
char* get_index(char *data, int index) {
	ELF64_FileRef  file;
	ELF64_HeaderRef header = NULL;
	file = ELF64_ReadFromData(data);
	if(ELF64_FileIsValid(file)) header = ELF64_FileGetHeader(file);
	ELF64_SectionHeaderEntryRef section; //=  get_index(data, 4);

	section = get_offset(&file,index); //ERROR;
	ELF64_ProgramHeaderEntryRef pheader = get_program_entry_ref(&file, index)	;
	char *nic = (char*)pheader;
	ELF64_SymbolTableEntryRef entry = mk_sym(&file, section, index);
	//printf("%d\n",ELF64_SymbolTableEntryGetNameOffset(entry));

	struct program_header_specs *specs = malloc(1000);
	int xx = 0;
	specs->Attrs = &xx;
	if(header==NULL) 
		return (char*)&err_return;
	create_hdr_specs(pheader, &specs);
	ELF64_SectionHeaderEntryRef hdr = (ELF64_SectionHeaderEntryRef )get_offset_h(&file, index);
	//printf("%d\n",*specs->Attrs);
        //struct entry_stuff *ent = get_entry(rice);
	if(section==NULL) return "fuck";
	const char *fsection = ELF64_FileGetNameOfSection(file, section);
	return (char*)fsection;

}
int check_type_check(FILE *f) {
	char *data = malloc(4006);
	char *buf; 
	buf = data;
	int i=1;
	void *fd; 
	while(1) {
		i+=1;
		if  ((fd = fread(buf, 1,1,f)) == NULL) return i;
	
	}
	free(data);
	return 0;
	
}
int main(int arg, char **argv) {
	char *data = malloc(40960);
	char *buf; //malloc(4000);
	buf = data;
	FILE *f = fopen(argv[1], "r")	;
	int offset=1;
	//while(fread(buf, 1,1, f)) {
	//	offset+=1;
	//	data = realloc(data, offset);
	//	buf = data + (offset-1) ;// + (offset-1);
	//}
	//int x = check_type_check(f);
	//f = realloc(f, x);
	while(1) {
		ssize_t rd = fread(buf, 1,1, f);
		offset+=1;
		data = realloc(data, offset);
		buf = data + (offset-1);
		if(!rd)  break;

	}
	char *getindex = get_index(data, 4);
	return (*(int*)getindex==2) ? 2 : 0;
	//printf("%s",get_index(data, 7));

}
