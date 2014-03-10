#ifndef CCTWTCL_COMMANDS_H
#define CCTWTCL_COMMANDS_H

#include <tcl.h>

int Cctwtcl_Initialize();

int Cctwtcl_Cmd               (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//int Cctwtcl_Count_Cmd         (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//int Cctwtcl_ijk2id_Cmd        (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

int Cctwtcl_Dependencies_Cmd  (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

int Cctwtcl_Input_Cmd         (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Transform_Cmd     (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Merge_Cmd         (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Normalize_Cmd     (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Output_Cmd        (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

int Cctwtcl_Delete_Cmd        (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Blob_Cmd          (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Blob_Info_Cmd     (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

/*
 *  Commands to implement a basic interface to cctw from swift/t via tcl
 *
 *  The following tcl commands are implemented:
 *
 *  cctw <cmd>
 *
 *    Passes javascript commands direct to the cctew script interpreter
 *
 *    Example:   cctw inputData.chunkSize
 *          -> 128,128,128
 *  cctw_count
 *
 *    Returns a list of two integers - the number of chunks in the input and output
 *    datasets respectively
 *
 *  cctw_input <filePath> <inputchunkid>
 *
 *    Reads a chunk of input data, normalizes it and performs any necessary corrections
 *    The filePath is interpreted as a URI - though the precise semantics are still to
 *    be determined.
 *
 *    Returns a list of three integers - the chunkid, the chunk length in bytes and
 *    the location of the chunk
 *
 *    Example:   cctw_input h5:example.h5/ffscan 1234
 *          ->   1234 33554472 6721372160
 *               cctw_input h5:example.h5/ffscan 1235
 *          ->   1235 33554472 6889164800
 *
 *  cctw_transform <inputchunkid> <blob>
 *
 *    Performs a cctw transform of a chunk of input data to produce a list of intermediate
 *    transformed output chunks.
 *
 *    Returns a list of lists of three integers - the chunkid, lengh and location of each intermediate chunk
 *
 *    Example:   cctw_transform 1234 6721372160
 *          -> {48 33554472 6754930688} {304 33554472 6788489216} {305 33554472 6822047744} {560 33554472 6855606272}
 *               cctw_transform 1235 6889164800
 *          -> {48 33554472 6922723328} {304 33554472 6956281856} {560 33554472 6989840384}
 *
 *  cctw_merge <outputchunkid> <blob1> <blob2>
 *
 *    Merges together two intermediate chunks with the same chunk id to produce a summed chunk
 *
 *    Returns a list of three integers - the chunkid, the chunk length in bytes and
 *    the location of the chunk
 *
 *    Example:   cctw_merge 304 6788489216 6956281856
 *          -> 304 33554472 7023398912
 *
 *  cctw_normalize <outputchunkid> <blob>
 *
 *    Normalizes an intermediate chunk by dividing the data values by the weight values and produces an output chunk
 *
 *    Returns a list of three integers - the chunkid, length and location of the output chunk
 *
 *    Example:   cctw_normalize 304 7023398912
 *          -> 304 16777256 7056957440
 *
 *  cctw_output <filePath> <outputchunkid> <blob>
 *
 *    Writes an output blob of data into the output file
 *    The filePath is interpreted as a URI - semantics to be determined
 *
 *    Example:  cctw_output h5:output.h5/ffscan 304 7056957440
 *
 *  cctw_delete <chunkid> <blob>
 *
 *    Should be called to delete each allocated blob
 *
 *    The chunkid should be an input or output chunkid as appropriate
 *
 *    Example: cctw_delete 304 7023398912
 *
 *  cctw_blob <chunkid> <blob>
 *
 *    Can be used to extract data out of a cctw blob into a tcl binary array
 *
 *    Returns three values - the chunkid, the length and the binary array
 *
 *    The binary array argument is suitable for use with the tcl binary command
 *
 *    Example: set x [cctw_blob 48 6754930688]
 *         -> 48 33554472 {0ÂÂÂ  }
 *             binary scan [lindex $x 2] iiiiii a b c d e f
 *         -> 6
 *             puts $d
 *         -> 128
 *
 *    The blobs have a common format - the first 8 integer values are
 *      0:  blob type    0 = input, 1 = intermediate, 2 = output
 *      1:  blob id
 *      2:  blob x dimension
 *      3:  blob y dimension
 *      4:  blob z dimension
 *      5:  number of data elements
 *      6:  number of weight elements (or 0 for an output blob)
 *      7:  alignment blank
 *
 *    The blob data then follows as an array of doubles
 *
 *  cctw_blob_info <chunkid> <blob>
 *
 *    Returns info about a blob as a keyed list
 *
 **/

#endif // CCTWTCL_COMMANDS_H
