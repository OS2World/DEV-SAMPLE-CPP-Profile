#define INCL_WINWINDOWMGR
#define INCL_WINSHELLDATA
#define INCL_DOSMISC
#include <os2.h>

class Profile {
        Profile(const char *profileFilename);
        virtual ~Profile();


        //binary---
        Bool Read (const char *group, const char *key
                   void *buf, int buflen
                  ) const;
        Bool Write(const char *group, const char *key,
                   const void *buf, int buflen
                  );
        int  Size (const char *group, const char *key
                  ) const;
        //asciiz---
        Bool Read (const char *group, const char *key,
                   char *buf, int buflen
                  ) const;
        Bool Write(const char *group, const char *key,
                   const char *buf
                  );
        //integrals---
        //short
        Bool Read (const char *group, const char *key,
                   short *s
                  ) const;
        Bool Write(const char *group, const char *key,
                   short s
                  );
        Bool Read (const char *group, const char *key,
                   unsigned short *us
                  ) const;
        Bool Write(const char *group, const char *key,
                   unsigned short us
                  );
        //int
        Bool Read (const char *group, const char *key,
                   int *i
                  ) const;
        Bool Write(const char *group, const char *key,
                   int i
                  );
        Bool Read (const char *group, const char *key,
                   unsigned *u
                  ) const;
        Bool Write(const char *group, const char *key,
                   unsigned u
                  );
        //long
        Bool Read (const char *group, const char *key,
                   long *l
                  ) const;
        Bool Write(const char *group, const char *key,
                   long l
                  );
        Bool Read (const char *group, const char *key,
                   unsigned long *ul
                  ) const;
        Bool Write(const char *group, const char *key,
                   unsigned long ul
                  );


        //iteration
        class GroupIterator {
                GroupIterator(const Profile &prf);
                operator int() const;
                const char *current() const;
                void advance();
        };
        class KeyIterator {
                KeyIterator(const Profile &prf, const char *group);
                operator int() const;
                const char *current();
                void advance();
        };
private:
        HINI hini;
};



Profile::Profile(const char *profileFilename) {
        char drive[_MAX_DRIVE],dir[_MAXX_DIR],name[_MAX_FNAME],ext[_MAX_EXT];
        _splitpath(profileFilename,drive,dir,name,ext);
        if(drive[0]=='\0' && dir[0]=='\0') {
                //no path given - use path specified by 'PROGRAM_INI_LOCATION'
                char path[_MAX_PATH];
                ULONG size;
                if(PrfQueryProfileSize(HINI_USER, "INIfiles","PROGRAM_INI_LOCATION", &ul)==FALSE || size==0) {
                        ULONG bootDrive;
                        DosQuerySysInfo(QSV_BOOT_DRIVE,QSV_BOOT_DRIVE,&bootDrive,sizeof(bootDrive));
                        path[0]='A'+(char)bootDrive;
                        strcat(path+1,":\\OS2");
                        PrfWriteProfileString(HINI_USER,"INIfiles","PROGRAM_INI_LOCATION",path);
                } else {
                        PrfQueryProfileData(HINI_USER,"INIFies,"PROGRAM_INI_LOCATION",path,_MAX_PATH);
                }

        } else
                hini=PrfOpenProfile(hab,profileFilename);
        }
}

Profile::~Profile() {
        if(hini)
                PrfCloseProfile(hini);
}


Bool Profile::Read (const char *group, const char *key
                    void *buf, int buflen
                   ) const;
Bool Profile::Write(const char *group, const char *key,
                    const void *buf, int buflen
                   );
int  Profile::Size (const char *group, const char *key
                   ) const
{
        ULONG size;
        if(PrfQueryProfileSize(group,key,&size)==FALSE)
                return -1;
        else
                return (int)size;
}

Bool Profile::Read (const char *group, const char *key,
                    char *buf, int buflen
                   ) const
{
        ULONG size;
        if(PrfQueryProfileSize(group,key,&size)==FALSE)
                return False;   //does not exist
        if(size==0)
                return False;   //0=can't be asciiz
        if(size>buflen)
                return False;   //to many characters
        if(PrfQueryProfileData(group,key,buf,buflen)==FALSE)
                return False;   //error occured
        if(buf[size-1]!='\0')
                return False;   //not asciiz
        return True;
}

Bool Profile::Write(const char *group, const char *key,
                    const char *buf
                   )
{
        return Write(group,key,(void*)buf,strlen(buf)+1);
}

Bool Profile::Read (const char *group, const char *key,
                    short *s
                   ) const
{
        long l;
        if(!Read(group,key,&l))
                return False;
        if(l<SHRT_MIN || l>SHRT_MAX)
                return False;
        *s=(short)l;
        return True;
}

Bool Profile::Write(const char *group, const char *key,
                    short s
                   );
{
        return Write(group,key,(long)s);
}

Bool Profile::Read (const char *group, const char *key,
                    unsigned short *us
                   ) const
{
        long ul;
        if(!Read(group,key,&ul))
                return False;
        if(ul>USHRT_MAX)
                return False;
        *us=(unsigned short)ul;
        return True;
}

Bool Profile::Write(const char *group, const char *key,
                    unsigned short us
                   )
{
        return Write(group,key,(unsigned long)us);
}

Bool Profile::Read (const char *group, const char *key,
                    int *i
                   ) const
{
        long l;
        if(!Read(group,key,&l))
                return False;
        if(l<INT_MIN || l>INT_MAX)
                return False;
        *s=(short)l;
        return True;
}

Bool Profile::Write(const char *group, const char *key,
                    int i
                   )
{
        return Write(group,key,(long)i);
}

Bool Profile::Read (const char *group, const char *key,
                    unsigned int *u
                   ) const
{
        long ul;
        if(!Read(group,key,&ul))
                return False;
        if(ul>UINT_MAX)
                return False;
        *u=(unsigned int)l;
        return True;
}

Bool Profile::Write(const char *group, const char *key,
                    unsigned u
                   )
{
        return Write(group,key,(unsigned long)i);
}

Bool Profile::Read (const char *group, const char *key,
                    long *l
                   ) const;

Bool Profile::Write(const char *group, const char *key,
                    long l
                   )
{
        return Write(group,key,&l,sizeof(l));
}

Bool Profile::Read (const char *group, const char *key,
                    unsigned long *ul
                   ) const
{
        return Read(group,key,(void*)ul,sizeof(*ul));
}

Bool Profile::Write(const char *group, const char *key,
                    unsigned long ul
                   )
{
        return Write(group,key,&ul,sizeof(ul));
}




void main(void) {
        HAB hab=WinInitialize(0);

        WinTerminate(hab);
}

