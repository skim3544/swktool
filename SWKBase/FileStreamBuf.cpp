#include "pch.h"
#include "FileStreamBuf.h"
#include "OpenOptions.h"

namespace swktool 
{
    /////////////////////////////////////////////////////////////////////
    FileStreamBuf::FileStreamBuf()
    {
        ::GetLocalTime(&LastTime_);
    }

    FileStreamBuf::FileStreamBuf(LPCTSTR fname)
    {
        File_ = OpenOptions()
            .Access(FileAccess::WriteData)
            .Share(FileShareMode::Read | FileShareMode::Write)
            .Disposition(CreationDisposition::OpenAlways)
            .Open(fname);

        // append data by forwarding the file pointer to the end of the file
        LARGE_INTEGER itg{ 0,0 };
        File_.Seek(itg, FileSeekMethod::END);

        ::GetLocalTime(&LastTime_);
    }

} //namespace swktool 