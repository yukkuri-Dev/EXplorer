
#include <stddef.h>
#include <unistd.h>

#define NALLOC 	512

typedef long Align;

union header {
    struct {
	union header	*ptr;
	size_t 	size;
    } s;
    Align x;
};

typedef union header Header;

static Header base;
static Header *freep = NULL;


void __libexword_free(void *ptr);

static Header *morecore(size_t nu) {
    char *cp;
    Header *up;
    if (nu < NALLOC)
	nu = NALLOC;
    cp = (char*)sbrk(nu * sizeof(Header));	
    if (cp == (char *) -1)
	return NULL;
    up = (Header*) cp;
    up->s.size = nu;
    __libexword_free((void*)(up+1));
    return freep;
}

void* __libexword_malloc(size_t size)
{
    Header *p, *prevp;
    size_t nunits;

    if (size == 0)
        return NULL;

    nunits = (size+sizeof(Header)-1)/sizeof(Header) + 1;

    if ((prevp = freep) == NULL) {
	base.s.ptr = freep = prevp = &base;
	base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {

	if (p->s.size >= nunits) {
	    if (p->s.size == nunits)
		prevp->s.ptr = p->s.ptr;
	    else {
		p->s.size -= nunits;
		p += p->s.size;
		p->s.size = nunits;
	    }
	    freep = prevp;
	    return (void*) (p+1);
	}
	if ( p == freep)
	    if ((p = morecore(nunits)) == NULL)
		return NULL;
    }
}


void __libexword_free(void *ptr)
{
    Header *bp, *p;
    
    if (ptr == NULL)
        return;

    bp = (Header*) ptr - 1;

    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
	if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
	    break;

    if (bp + bp->s.size == p->s.ptr) {	
	bp->s.size += p->s.ptr->s.size;
	bp->s.ptr = p->s.ptr->s.ptr;
    }
    else
	bp->s.ptr = p->s.ptr;
    if ( p + p->s.size == bp ) {
	p->s.size += bp->s.size;
	p->s.ptr = bp->s.ptr;
    } else
	p->s.ptr = bp;
    freep = p;
}

