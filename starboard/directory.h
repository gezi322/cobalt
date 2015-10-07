#ifndef STARBOARD_DIRECTORY_H_
#define STARBOARD_DIRECTORY_H_

#include "starboard/configuration.h"
#include "starboard/export.h"
#include "starboard/file.h"
#include "starboard/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Private structure representing an open directory stream.
struct SbDirectoryPrivate;

// A handle to an open directory stream.
typedef struct SbDirectoryPrivate *SbDirectory;

// Represents a directory entry.
typedef struct SbDirectoryEntry {
  // The name of this directory entry.
  char name[SB_FILE_MAX_NAME];
} SbDirectoryEntry;


// Well-defined value for an invalid directory stream handle.
const SbDirectory kSbDirectoryInvalid = (SbDirectory)kSbInvalidPointer;

// Returns whether the given directory stream handle is valid.
SB_C_INLINE bool SbDirectoryIsValid(SbDirectory directory) {
  return directory != kSbDirectoryInvalid;
}

// Opens the given existing directory for listing. Will return
// kSbDirectoryInvalidHandle if not successful. If |out_error| is provided by
// the caller, it will be set to the appropriate SbFileError code on failure.
SB_EXPORT SbDirectory SbDirectoryOpen(
    const char *path,
    SbFileError *out_error);

// Closes an open directory stream handle. Returns whether the close was
// successful.
SB_EXPORT bool SbDirectoryClose(SbDirectory directory);

// Populates |out_entry| with the next entry in that directory stream, and moves
// the stream forward by one entry. Returns |true| if there was a next
// directory, and |false| at the end of the directory stream.
SB_EXPORT bool SbDirectoryGetNext(SbDirectory directory,
                                  SbDirectoryEntry *out_entry);

// Returns whether SbDirectoryOpen is allowed for the given |path|.
SB_EXPORT bool SbDirectoryCanOpen(const char *path);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // STARBOARD_DIRECTORY_H_
