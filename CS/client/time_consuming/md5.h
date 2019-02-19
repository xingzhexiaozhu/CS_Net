/* MD5.H - header file for MD5C.C
 */
typedef char BYTE;
typedef char BOOL;
#define TRUE 1
#define FALSE 0
//extern "C" {
    void Md5HashBuffer( BYTE *outBuffer, const BYTE *inBuffer, int length);
    BOOL Md5Test( void );
//}
