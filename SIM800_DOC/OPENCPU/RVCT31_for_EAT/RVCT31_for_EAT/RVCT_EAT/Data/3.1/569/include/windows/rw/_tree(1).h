/***************************************************************************
 *
 * _tree.h - Declarations for the Standard Library tree classes
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _tree.h 91721 2005-08-08 18:30:07Z drodgman $
 *
 ***************************************************************************
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 ***************************************************************************
 *
 * Copyright (c) 1994-2001 Rogue Wave Software, Inc.  All Rights Reserved.
 *
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software,
 * Inc. pursuant to a written license agreement and may be used, copied,
 * transmitted, and stored only in accordance with the terms of such
 * license and with the inclusion of the above copyright notice.  This
 * computer software or any other copies thereof may not be provided or
 * otherwise made available to any other person.
 *
 * U.S. Government Restricted Rights.  This computer software is provided
 * with Restricted Rights.  Use, duplication, or disclosure by the
 * Government is subject to restrictions as set forth in subparagraph (c)
 * (1) (ii) of The Rights in Technical Data and Computer Software clause
 * at DFARS 252.227-7013 or subparagraphs (c) (1) and (2) of the
 * Commercial Computer Software--Restricted Rights at 48 CFR 52.227-19,
 * as applicable.  Manufacturer is Rogue Wave Software, Inc., 5500
 * Flatiron Parkway, Boulder, Colorado 80301 USA.
 *
 **************************************************************************/

/*
**
** Red-black tree class, designed for use in implementing STL
** associative containers (set, multiset, map, and multimap). The
** insertion and deletion algorithms are based on those in Cormen,
** Leiserson, and Rivest, Introduction to Algorithms (MIT Press, 1990),
** except that:
** 
** (1) the header cell is maintained with links not only to the root
** but also to the leftmost node of the tree, to enable constant time
** begin(), and to the rightmost node of the tree, to enable linear time
** performance when used with the generic set algorithms (set_union,
** etc.);
** 
** (2) when a node being deleted has two children its successor node is
** relinked into its place, rather than copied, so that the only
** iterators invalidated are those referring to the deleted node.
** 
*/

#ifndef _RWSTD_TREE_H_INCLUDED
#define _RWSTD_TREE_H_INCLUDED


#include <rw/_algobase.h>
#include <rw/_iterator.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE_BEGIN (__rw)

template <class _Alloc, class _Val, class _Key, class _KeyOf>
struct __rw_rb_tree_node {

    enum _C_color_t { _C_red, _C_black };

    typedef _Val&                                          reference;
    typedef _Alloc                                         allocator_type;

    typedef _RWSTD_REBIND (allocator_type, __rw_rb_tree_node) _C_node_alloc_t;
    typedef _RWSTD_REBIND (allocator_type, _Key)              _C_key_alloc_t;

    typedef _TYPENAME _C_node_alloc_t::pointer             _C_link_t;
    typedef _TYPENAME _C_key_alloc_t::const_reference      _C_const_key_ref;

    _C_color_t   _C_color_field; 
    _C_link_t    _C_parent_link;
    _C_link_t    _C_left_link;
    _C_link_t    _C_right_link;
    _Val         _C_value_field;

    static _C_link_t _C_minimum (_C_link_t __x) {
        _RWSTD_ASSERT (0 != (void*)__x);
        while ((void*)__x->_C_left())
            __x = __x->_C_left();
        return __x;
    }
    
    static _C_link_t _C_maximum (_C_link_t __x) {
        _RWSTD_ASSERT (0 != (void*)__x);
        while ((void*)__x->_C_right())
            __x = __x->_C_right();
        return __x;
    }

    _C_link_t& _C_left() {
        return _C_left_link;
    }
    
    _C_link_t& _C_right () {
        return _C_right_link;
    }
    
    _C_link_t& _C_parent () {
      return _C_parent_link;
    }
    
    reference _C_value () {
        return _C_value_field;
    }
    
    _C_const_key_ref _C_key () {
        return _KeyOf()(_C_value_field);
    }
    
    _C_color_t& _C_color () {
        return _C_color_field;
    }
    
};


template <class _TypeT, class _DiffT,
          class _Pointer, class _Reference, class _Node>
class __rw_tree_iter
    : public _STD::iterator <_STD::bidirectional_iterator_tag,
                            _TypeT, _DiffT, _Pointer, _Reference>
{
    typedef _STD::iterator <_STD::bidirectional_iterator_tag,
                           _TypeT, _DiffT, _Pointer, _Reference> _C_iter_base;
public:

    typedef _TYPENAME _C_iter_base::value_type        value_type;
    typedef _TYPENAME _C_iter_base::difference_type   difference_type;
    typedef _TYPENAME _C_iter_base::pointer           pointer;
    typedef _TYPENAME _C_iter_base::reference         reference;
    typedef _TYPENAME _C_iter_base::iterator_category iterator_category;
    typedef _Node                                     _C_node_t;
    typedef _TYPENAME _C_node_t::allocator_type       allocator_type;
    typedef _TYPENAME _C_node_t::_C_link_t            _C_link_t;

    typedef const value_type*                         const_pointer; 
    typedef const value_type&                         const_reference; 

    typedef __rw_tree_iter<_TypeT, _DiffT, value_type*, value_type&, _C_node_t>
    _C_iterator;

    _C_link_t _C_node;

    __rw_tree_iter () {}

    // no copy ctor other than the one below is defined
    // will use a compiler generated one if __rw_tree_iter != _C_iterator
    __rw_tree_iter (const _C_iterator &__rhs)
        : _C_node (__rhs._C_node) { }

#ifndef _RWSTD_NO_MEMBER_TEMPLATES

    template <class _Ptr, class _Ref>
    __rw_tree_iter (const __rw_tree_iter<_TypeT, _DiffT, _Ptr, _Ref, _Node>&
                    __rhs)
        : _C_node (__rhs._C_node) { }

#endif   // _RWSTD_NO_MEMBER_TEMPLATES

    __rw_tree_iter (_C_link_t __x)
        : _C_node(__x) {}        

#ifdef SNI
    difference_type operator- (const __rw_tree_iter&) const {
        return 0;
    }
#endif
    
    __rw_tree_iter& operator++ () {
        if ((void*)_C_node->_C_right()) {
            _C_node = _C_node->_C_right();
            while ((void*)_C_node->_C_left())
                _C_node = _C_node->_C_left();
        }
        else {
            _C_link_t __y = _C_node->_C_parent();
            while (_C_node == __y->_C_right()) {
                _C_node = __y; __y = __y->_C_parent();
            }
            if (_C_node->_C_right() != __y) // Necessary because of rightmost.
                _C_node = __y;
        }
        return *this;
    }
    
    __rw_tree_iter& operator-- () {
        if (_C_node->_C_color() == _C_node_t::_C_red
            && _C_node->_C_parent()->_C_parent() == _C_node)  
            //
            // Check for header.
            //
            _C_node = _C_node->_C_right();   // Return rightmost.
        else if ((void*)_C_node->_C_left()) {
            _C_link_t __y = _C_node->_C_left();
            while ((void*)__y->_C_right())
                __y = __y->_C_right();
            _C_node = __y;
        }
        else {
          _C_link_t __y = _C_node->_C_parent();
          while (_C_node == __y->_C_left()) {
            _C_node = __y; __y = __y->_C_parent();
          }
          _C_node = __y;
        }
        return *this;
    }

    __rw_tree_iter operator++ (int) {
        __rw_tree_iter __tmp = *this;
        return ++*this, __tmp;
    }
    
    __rw_tree_iter operator-- (int) {
        __rw_tree_iter __tmp = *this;
        return --*this, __tmp;
    }

    reference operator* () const {
        return _C_node->_C_value();
    }

    _RWSTD_OPERATOR_ARROW (pointer operator-> () const);
};


#define _RWSTD_TREE_ITER(n) \
        __rw_tree_iter <_TypeT, _DiffT, _Ptr##n, _Ref##n, _Node>

template <class _TypeT, class _DiffT,
          class _Ptr1, class _Ref1, class _Ptr2, class _Ref2, class _Node>
inline bool
operator== (const _RWSTD_TREE_ITER(1)  &__x, const _RWSTD_TREE_ITER(2)  &__y)
{
    return __x._C_node == __y._C_node;
}

template <class _TypeT, class _DiffT,
          class _Ptr1, class _Ref1, class _Ptr2, class _Ref2, class _Node>
inline bool
operator!= (const _RWSTD_TREE_ITER(1) &__x, const _RWSTD_TREE_ITER(2) &__y)
{
    return !(__x == __y);
}


#undef _RWSTD_TREE_ITER


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
class __rb_tree : private _Alloc
{
protected:
    
    typedef __rw_rb_tree_node<_Alloc,_Val,_Key,_KeyOf> _C_node_t; 
    typedef _RWSTD_ALLOC_TYPE (_Alloc,_Val)            _C_val_alloc_t;
    typedef _TYPENAME _C_node_t::_C_key_alloc_t        _C_key_alloc_t;
    typedef _TYPENAME _C_node_t::_C_node_alloc_t       _C_node_alloc_t;
    typedef _TYPENAME _C_node_t::_C_link_t             _C_link_t;

public:
    
    typedef _Key                                       key_type;
    typedef _Val                                       value_type;
    typedef _Alloc                                     allocator_type;
      
    typedef _TYPENAME _C_val_alloc_t::pointer          pointer;
    typedef _TYPENAME _C_val_alloc_t::const_pointer    const_pointer;
    typedef _TYPENAME _Alloc::size_type                size_type;
    typedef _TYPENAME _Alloc::difference_type          difference_type;
    typedef _TYPENAME _C_val_alloc_t::reference        reference;
    typedef _TYPENAME _C_val_alloc_t::const_reference  const_reference;
    
protected:

    struct _C_rb_tree_node_buffer;
    friend struct _C_rb_tree_node_buffer;

    // for brevity, long name preserved for ABI compatibility
    typedef _C_rb_tree_node_buffer _C_node_buf;
    
    typedef _RWSTD_REBIND(allocator_type,_C_node_buf) _C_buf_alloc_t;
    typedef _TYPENAME _C_buf_alloc_t::pointer _C_buf_ptr_t;

    typedef __rw_tree_iter<value_type, difference_type, pointer,
                              reference, _C_node_t>        _C_tree_iter;

    typedef __rw_tree_iter<value_type, difference_type, const_pointer,
                              const_reference, _C_node_t>  _C_tree_citer;

public:

#ifndef _RWSTD_NO_DEBUG_ITER

    typedef __rw_debug_iter <__rb_tree, _C_tree_iter, _C_tree_iter>
    iterator;

    typedef __rw_debug_iter <__rb_tree, _C_tree_citer, _C_tree_iter>
    const_iterator;

    iterator _C_make_iter (_C_link_t __node) {
        return iterator (*this, _C_tree_iter (__node));
    }

    const_iterator _C_make_iter (const _C_link_t __node) const {
        return const_iterator (*this, _C_tree_citer (__node));
    }

#else   // if defined (_RWSTD_NO_DEBUG_ITER)

    typedef _C_tree_iter         iterator;
    typedef _C_tree_citer        const_iterator;

    iterator _C_make_iter (_C_link_t __node) {
        return iterator (__node);
    }

    const_iterator _C_make_iter (const _C_link_t __node) const {
        return const_iterator (__node);
    }

#endif   // _RWSTD_NO_DEBUG_ITER

protected:

    struct _C_rb_tree_node_buffer {
        _C_buf_ptr_t _C_next_buffer;
        size_type    size;
        _C_link_t    _C_buffer;
    };
    
    _C_buf_ptr_t _C_buf_list;
    _C_link_t    _C_free_list;
    _C_link_t    _C_next_avail;
    _C_link_t    _C_last;
    
    void _C_add_new_buffer () {
        _RWSTD_C::size_t __next_buffer_size = 0;
        if ((void*)_C_buf_list) {
            __next_buffer_size = 
                _RW::__rw_new_capacity(_C_buf_list->size,this);
        }
        else {
            __next_buffer_size = 
                _RW::__rw_new_capacity(0,this);
        }          
        _C_buf_ptr_t __tmp = 
            _C_buf_alloc_t(*this).
                allocate(_RWSTD_STATIC_CAST(size_type,1), (void*)_C_buf_list);
#ifndef _RWSTD_NO_EXCEPTIONS
        try {
            __tmp->_C_buffer = 
                _C_node_alloc_t(*this).
                    allocate(__next_buffer_size, (void*)_C_last);
        } catch(...) {
            _C_buf_alloc_t(*this).deallocate(__tmp,1);
            throw;
        } 
#else
        __tmp->_C_buffer = 
            _C_node_alloc_t(*this).allocate(__next_buffer_size,_C_last);
#endif //  _RWSTD_NO_EXCEPTIONS     
        __tmp->_C_next_buffer   = _C_buf_list;
        __tmp->size             = __next_buffer_size;
        _C_buf_list             = __tmp;
        _C_next_avail           = _C_buf_list->_C_buffer;
        _C_last                 = _C_next_avail + __next_buffer_size;
    }
    void _C_deallocate_buffers ();
    
    // 
    // Return a node from the free list or new storage
    //
    _C_link_t _C_get_link() {
        _C_link_t __tmp = _C_free_list;
        _C_link_t __tmp2 = (void*)_C_free_list ? 
            (_C_free_list = _RWSTD_STATIC_CAST(_C_link_t,(_C_free_list->_C_right_link)), __tmp) 
            : (_C_next_avail == _C_last ? (_C_add_new_buffer(), _C_next_avail++) 
               : _C_next_avail++);
        __tmp2->_C_parent_link = NULL;
        __tmp2->_C_left_link = NULL;
        __tmp2->_C_right_link = NULL;
        __tmp2->_C_color_field = _C_node_t::_C_red;
        return __tmp2;
    }

    //
    // Return a node from the free list or new storage with
    // the _Val __v constructed on it.  Every call to _C_get_node
    // must eventually be followed by a call to _C_put_node.
    //
    _C_link_t _C_get_node (const _Val& __v) {
        _C_link_t __tmp2 = _C_get_link();
#ifndef _RWSTD_NO_EXCEPTIONS
        try {
            _RWSTD_VALUE_ALLOC
                (_C_val_alloc_t, 
                 construct(_RWSTD_VALUE_ALLOC (_C_val_alloc_t,
                                               address(__tmp2->_C_value())),
                                               __v));
        } catch(...) {
            _C_put_node(__tmp2,false);
            throw;
        }      
#else
        _RWSTD_VALUE_ALLOC
            (_C_val_alloc_t, 
             construct(_RWSTD_VALUE_ALLOC (_C_val_alloc_t,
                                           address(__tmp2->_C_value())), __v));
#endif // _RWSTD_NO_EXCEPTIONS
        return __tmp2;
    }
    _C_link_t _C_get_node () {
        return _C_get_link();
    }
    
    // 
    // Return a node to the free list and destroy the value in it.
    //
    void _C_put_node (_C_link_t __p, bool do_destroy = true) { 
        __p->_C_right_link = _C_free_list; 
        if (do_destroy)  {
            _RWSTD_VALUE_ALLOC (_C_val_alloc_t, 
                                destroy (_RWSTD_VALUE_ALLOC (_C_val_alloc_t,
                                                   address (__p->_C_value()))));
        }
        _C_free_list = __p; 
    }
    
protected:

    _C_link_t  _C_header;  

    _C_link_t& _C_root      () const { return _C_header->_C_parent (); }
    _C_link_t& _C_leftmost  () const { return _C_header->_C_left ();   }
    _C_link_t& _C_rightmost () const { return _C_header->_C_right();   }
    
    size_type  _C_node_count;    // Keeps track of size of tree.
    bool       _C_insert_always; // Controls whether an element already in the
                                 // tree is inserted again.
    _Comp      _C_key_comp;

public:
    

#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC

    typedef _STD::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef _STD::reverse_iterator<iterator>       reverse_iterator;

#else

    typedef __reverse_bi_iterator<const_iterator, 
        _STD::bidirectional_iterator_tag, value_type, 
        const_reference, const_pointer, difference_type>
    const_reverse_iterator;

    typedef __reverse_bi_iterator<iterator, 
        _STD::bidirectional_iterator_tag, value_type,
        reference, pointer, difference_type>
    reverse_iterator;

#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC

  private:

    iterator     _C_insert (_C_link_t __x, _C_link_t __y, 
                            const value_type& __v);
    _C_link_t _C_copy   (_C_link_t __x, _C_link_t __p);
    void         _C_erase  (_C_link_t __x);
    inline void  _C_erase_leaf  (_C_link_t __x);

    void _C_init ()
    {
      _C_buf_list    = 0;
      _C_free_list   = _C_next_avail = _C_last = 0;
      _C_header      = _C_get_node();
      _C_root()      = 0;
      _C_leftmost()  = _C_header;
      _C_rightmost() = _C_header;
    }

  public:

    __rb_tree (const _Comp& _RWSTD_COMP = _Comp(),
               bool __always = true,
               const _Alloc& __alloc =  _Alloc()) 
      : allocator_type (__alloc),_C_buf_list(0), _C_header(0), _C_node_count(0),
        _C_insert_always(__always), _C_key_comp(_RWSTD_COMP)
    {
      _C_init();
    }

#ifndef _RWSTD_NO_MEMBER_TEMPLATES
    template<class _InputIter>
    __rb_tree (_InputIter __first, _InputIter __last, 
             const _Comp& __cmp = _Comp(), bool __always = true,
             const _Alloc& __alloc = _Alloc())
      : allocator_type(__alloc),_C_buf_list(0), _C_header(0), _C_node_count(0), 
        _C_insert_always(__always), _C_key_comp(__cmp)
    { 
      _C_init(); 
#ifndef _RWSTD_NO_EXCEPTIONS
      try {
        insert(__first, __last);
      } catch(...) {
        _C_deallocate_buffers();
        throw;
      }
#else
      insert(__first, __last);
#endif // _RWSTD_NO_EXCEPTIONS
    }
#else
    __rb_tree (const value_type* __first, const value_type* __last, 
               const _Comp& _RWSTD_COMP = _Comp(),
               bool __always = true,
               const _Alloc& __alloc = _Alloc())
      : allocator_type(__alloc),_C_buf_list(0), _C_header(0), _C_node_count(0), 
        _C_insert_always(__always), _C_key_comp(_RWSTD_COMP)
    { 
      _C_init(); 
#ifndef _RWSTD_NO_EXCEPTIONS
      try {
        insert(__first, __last);
      } catch(...) {
        _C_deallocate_buffers();
        throw;
      }
#else
      insert(__first, __last);
#endif // _RWSTD_NO_EXCEPTIONS
    }
    __rb_tree (const_iterator __first, const_iterator __last, 
               const _Comp& _RWSTD_COMP = _Comp(),
               bool __always = true,
               const _Alloc& __alloc = _Alloc())
      : allocator_type(__alloc), _C_buf_list(0), _C_header(0), _C_node_count(0), 
        _C_insert_always(__always), _C_key_comp(_RWSTD_COMP)
    { 
      _C_init(); 
#ifndef _RWSTD_NO_EXCEPTIONS
      try {
        insert(__first, __last);
      } catch(...) {
        _C_deallocate_buffers();
        throw;
      }
#else
      insert(__first, __last);
#endif // _RWSTD_NO_EXCEPTIONS
    }
   
#endif

    __rb_tree (const __rb_tree<_Key,_Val,_KeyOf,_Comp,_Alloc>& __x,
             bool __always = true)
      : allocator_type(__x.get_allocator()), _C_buf_list(0), _C_header(0), 
        _C_node_count(__x._C_node_count), _C_insert_always(__always), 
        _C_key_comp(__x._C_key_comp)
    { 
      _C_free_list          = _C_next_avail = _C_last = 0;
      _C_header             = _C_get_node();
      _C_header->_C_color() = _C_node_t::_C_red;
      _TRY { 
          _C_root() = _C_copy(__x._C_root(), _C_header);
      }
      _CATCH (...) {
          _C_deallocate_buffers();
          _RETHROW;
      }
      if (_C_root()) {
        _C_leftmost()  = _C_node_t::_C_minimum(_C_root());
        _C_rightmost() = _C_node_t::_C_maximum(_C_root());
      }
      else {
        _C_leftmost()  = _C_header;
        _C_rightmost() = _C_header;
      }
    }

    ~__rb_tree () {
        if ((void*)_C_header) {
            erase (begin(), end ());
            _C_put_node (_C_header, false);
            _C_deallocate_buffers ();
      }
    }

    __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>& 
    operator= (const __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>& __x);

    _Comp     key_comp () const { return _C_key_comp; }
    _C_val_alloc_t get_allocator() const
    {
      return _C_val_alloc_t(*this);
    }

    iterator begin () {
        return _C_make_iter (_C_leftmost ());
    }

    const_iterator begin () const {
        return _C_make_iter (_C_leftmost ());
    }

    iterator end () {
        return _C_make_iter (_C_header);
    }

    const_iterator end () const {
        return _C_make_iter (_C_header);
    }

    reverse_iterator rbegin () { 
        return reverse_iterator(end());
    }
    
    reverse_iterator rend () { 
        return reverse_iterator (begin());
    } 

    const_reverse_iterator rbegin () const { 
        return const_reverse_iterator (end());
    }
    
    const_reverse_iterator rend () const { 
        return const_reverse_iterator(begin());
    } 

    bool      empty    () const {
        return _C_node_count == 0;
    }

    size_type size     () const {
        return _C_node_count;
    }

    size_type max_size () const { 
      return _C_node_alloc_t(*this).max_size(); 
    }
    
    void swap (__rb_tree &__t) {
      if(get_allocator() == __t.get_allocator()) {
          _STD::swap (_C_buf_list, __t._C_buf_list);
          _STD::swap (_C_free_list, __t._C_free_list);
          _STD::swap (_C_next_avail, __t._C_next_avail);
          _STD::swap (_C_last, __t._C_last);
          _STD::swap (_C_header, __t._C_header);
          _STD::swap (_C_node_count, __t._C_node_count);
          _STD::swap (_C_insert_always, __t._C_insert_always);
          _STD::swap (_C_key_comp, __t._C_key_comp);
      }
      else {
          __rb_tree __x = *this;
          *this = __t;
          __t = __x;
      } 
    }

    _STD::pair<iterator, bool> insert (const value_type&);

    iterator insert (iterator, const value_type&);

#ifndef _RWSTD_NO_MEMBER_TEMPLATES
    template<class _Iterator>
    void      insert (_Iterator __first, _Iterator __last);
#else
    void      insert (const_iterator __first, const_iterator __last);
    void      insert (const value_type* __first, const value_type* __last);
#endif

    iterator  erase  (iterator);
    size_type erase  (const key_type&);
    iterator  erase  (iterator, iterator);
    void      erase  (const key_type*, const key_type*);

    iterator find (const key_type&);

    const_iterator find (const key_type& __x) const {
        return _RWSTD_CONST_CAST (__rb_tree*, this)->find (__x);
    }

    size_type count (const key_type& __x) const;

    iterator lower_bound (const key_type& __x);

    const_iterator lower_bound (const key_type& __x) const {
        return _RWSTD_CONST_CAST (__rb_tree*, this)->lower_bound (__x);
    }

    iterator upper_bound (const key_type& __x);

    const_iterator upper_bound (const key_type& __x) const {
        return _RWSTD_CONST_CAST (__rb_tree*, this)->upper_bound (__x);
    }

    _STD::pair<iterator, iterator> equal_range (const key_type& __x);

    _STD::pair<const_iterator, const_iterator>
    equal_range (const key_type& __x) const {
        _STD::pair<iterator, iterator> __tmp =
            _RWSTD_CONST_CAST (__rb_tree*, this)->equal_range (__x);
        return _STD::pair<const_iterator, const_iterator>
            (__tmp.first, __tmp.second);
    }

    inline void __rotate_left  (_C_link_t __x);
    inline void __rotate_right (_C_link_t __x);

};


//
// Inline functions
//

template <class _Key, class _Val, class _KeyOf, 
class _Comp, class _Alloc>
inline bool
operator== (const __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>& __x, 
            const __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>& __y)
{
    return    __x.size () == __y.size ()
           && equal (__x.begin (), __x.end (), __y.begin ());
}


template <class _Key, class _Val, class _KeyOf, 
class _Comp, class _Alloc>
inline bool
operator< (const __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>& __x, 
           const __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>& __y)
{
    return lexicographical_compare (__x.begin (), __x.end (),
                                    __y.begin (), __y.end ());
}


template <class _Key,class _Val,class _KeyOf,class _Comp,class _Alloc>
inline void   
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::_C_erase_leaf (_C_link_t __x)
{
    // Remove a leaf node from the tree
    _C_link_t __y = __x->_C_parent();
    if (__y == _C_header)
    {
      _C_leftmost() = _C_rightmost() = __y;
      _C_root() = 0;
    }
    else if (__y->_C_left() == __x)
    {
      __y->_C_left() = 0;
      if (_C_leftmost() == __x)
        _C_leftmost() = __y;
    }
    else
    {
      __y->_C_right() = 0;
      if (_C_rightmost() == __x)
        _C_rightmost() = __y;
    }
  }


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
inline void 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::__rotate_left (_C_link_t __x)
{
    _RWSTD_ASSERT (0 != (void*)__x);

    _C_link_t __y   = __x->_C_right();
    __x->_C_right() = __y->_C_left();

    if ((void*)__y->_C_left ())
        __y->_C_left()->_C_parent() = __x;

    __y->_C_parent() = __x->_C_parent();

    if (__x == _C_root())
        _C_root() = __y;
    else if (__x == __x->_C_parent()->_C_left())
        __x->_C_parent()->_C_left() = __y;
    else
        __x->_C_parent()->_C_right() = __y;

    __y->_C_left ()  = __x;
    __x->_C_parent() = __y;
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
inline void 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::__rotate_right (_C_link_t __x)
{
    _RWSTD_ASSERT (0 != (void*)__x);
      
    _C_link_t __y   = __x->_C_left();
    __x->_C_left () = __y->_C_right();

    if ((void*)__y->_C_right ())
        __y->_C_right()->_C_parent() = __x;

    __y->_C_parent() = __x->_C_parent();
    
    if (__x == _C_root())
        _C_root() = __y;
    else if (__x == __x->_C_parent()->_C_right())
        __x->_C_parent()->_C_right() = __y;
    else
        __x->_C_parent()->_C_left() = __y;

    __y->_C_right ()  = __x;
    __x->_C_parent () = __y;
}


#ifndef _RWSTD_NO_MEMBER_TEMPLATES

template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
template<class _Iterator>
inline void __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::
insert (_Iterator __first, _Iterator __last)
{
    for (; __first != __last; ++__first)
        insert(*__first);
}

#else

template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
inline void __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::
insert (const_iterator __first, const_iterator __last)
{
    for (; __first != __last; ++__first)
        insert(*__first);
}

template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
inline void __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::
insert (const _Val* __first, const _Val* __last)
{
    for (; __first != __last; ++__first)
        insert(*__first);
}

#endif   // _RWSTD_NO_MEMBER_TEMPLATES
         

template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
void __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::
erase (const _Key* __first, const _Key* __last)
{
    for (; __first != __last; ++__first)
        erase(*__first);
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
inline _TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::size_type 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::count (const _Key& __k) const
{
    _STD::pair<const_iterator, const_iterator> __p = equal_range(__k);
    size_type __n = _DISTANCE (__p.first, __p.second, size_type);
    return __n;
}


#define _RWSTD_RB_TREE_ITER _TYPENAME __rb_tree<_Key, _Val, _KeyOf, \
                                               _Comp, _Alloc>::iterator
                                               
template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
inline _STD::pair<_RWSTD_RB_TREE_ITER , _RWSTD_RB_TREE_ITER >
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::equal_range (const _Key& __k)
{
    return _STD::pair<iterator, iterator>(lower_bound (__k), upper_bound (__k));
}

#undef _RWSTD_RB_TREE_ITER


_RWSTD_NAMESPACE_END   // __rw


#ifdef _RWSTD_COMPILE_INSTANTIATE
#  include <rw/_tree.cc>
#endif

#endif   // _RWSTD_TREE_H_INCLUDED
