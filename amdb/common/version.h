#pragma

#define AMDB_MAJOR "0"
#define AMDB_MINOR "1"
#define AMDB_PATCH "SNAPSHOT"

#define STR_I(var) #var
#define STR(var) STR_I(var)

static char const rcsid[] =
    ", built by gcc " STR(__GNUC__) "." STR(__GNUC_MINOR__) "." STR(__GNUC_PATCHLEVEL__)
      ", built at " __DATE__ " " __TIME__ " "
        ", Version: AmDB" AMDB_MAJOR "." AMDB_MINOR "." AMDB_PATCH " ,"
        " commit " GIT_COMMIT_ID " $";


const char* GetRcsid() { return rcsid; }