/***************************************************************************
 *
 * _tree.cc - Non-inline tree definitions for the Standard Library
 *
 * $Id: _tree.cc 91768 2005-08-10 18:58:48Z drodgman $
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

// for convenience
#define _ITER_NODE(it)   (_ITER_BASE (it)._C_node)


_RWSTD_NAMESPACE_BEGIN (__rw)


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
void __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::_C_deallocate_buffers ()
{
    while ((void*)_C_buf_list) {
        _C_buf_ptr_t __tmp = _C_buf_list;
        _C_buf_list        = (_C_buf_ptr_t)(_C_buf_list->_C_next_buffer);
        _C_node_alloc_t(*this).deallocate(__tmp->_C_buffer,__tmp->size);
        _C_buf_alloc_t(*this).deallocate(__tmp,1);
    }
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>& 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::
operator= (const __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>& __x)
{
    if (!(this == &__x)) {
      erase(begin(), end());
      _C_root() = _C_copy(__x._C_root(), _C_header);
      if (!(void*)_C_root()) {
        _C_leftmost()  = _C_header; _C_rightmost() = _C_header;
      }
      else {
        _C_leftmost()  = _C_node_t::_C_minimum(_C_root());
        _C_rightmost() = _C_node_t::_C_maximum(_C_root());
      }
      _C_node_count = __x._C_node_count;
    }
    return *this;
  }


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::iterator
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::
_C_insert (_C_link_t __x, _C_link_t __y, const _Val& __v)
{
    _C_link_t __z = _C_get_node(__v);
    ++_C_node_count;
    if (   __y == _C_header || (void*)__x
        || _C_key_comp(_KeyOf()(__v), __y->_C_key ())) {
        // Also makes _C_leftmost() = `z' when `y' == _C_header.
        __y->_C_left() = __z;
      if (__y == _C_header) {
        _C_root() = __z; _C_rightmost() = __z;
      }
      else if (__y == _C_leftmost())
        _C_leftmost() = __z;   // Maintain _C_leftmost() pointing to minimum node.
    }
    else {
      __y->_C_right() = __z;
      if (__y == _C_rightmost())
        _C_rightmost() = __z;  // Maintain _C_rightmost() pointing to maximum node.
    }
    __z->_C_parent() = __y;
    __x = __z;  // Recolor and rebalance the tree.
    while (__x != _C_root() && __x->_C_parent()->_C_color() == _C_node_t::_C_red) {
      if (__x->_C_parent() == __x->_C_parent()->_C_parent()->_C_left()) {
        __y = __x->_C_parent()->_C_parent()->_C_right();
        if ((void*)__y && __y->_C_color() == _C_node_t::_C_red) {
          __x->_C_parent()->_C_color()         = _C_node_t::_C_black;
          __y->_C_color()                 = _C_node_t::_C_black;
          __x->_C_parent()->_C_parent()->_C_color() = _C_node_t::_C_red;
          __x                        = __x->_C_parent()->_C_parent();
        }
        else {
          if (__x == __x->_C_parent()->_C_right()) {
            __x = __x->_C_parent(); 
            __rotate_left(__x);
          }
          __x->_C_parent()->_C_color() = _C_node_t::_C_black;
          __x->_C_parent()->_C_parent()->_C_color() = _C_node_t::_C_red;
          __rotate_right(__x->_C_parent()->_C_parent());
        }
      }
      else {
        __y = __x->_C_parent()->_C_parent()->_C_left();
        if ((void*)__y && __y->_C_color() == _C_node_t::_C_red) {
          __x->_C_parent()->_C_color()         = _C_node_t::_C_black;
          __y->_C_color()                 = _C_node_t::_C_black;
          __x->_C_parent()->_C_parent()->_C_color() = _C_node_t::_C_red;
          __x                        = __x->_C_parent()->_C_parent();
        }
        else {
          if (__x == __x->_C_parent()->_C_left()) {
            __x = __x->_C_parent(); 
            __rotate_right(__x);
          }
          __x->_C_parent()->_C_color()         = _C_node_t::_C_black;
          __x->_C_parent()->_C_parent()->_C_color() = _C_node_t::_C_red;
          __rotate_left(__x->_C_parent()->_C_parent());
        }
      }
    }
    _C_root()->_C_color() = _C_node_t::_C_black;
    return _C_make_iter (__z);
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_STD::pair<_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::iterator,
           bool>
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::insert (const _Val& __v)
{
    _C_link_t __y = _C_header;
    _C_link_t __x = _C_root();
    bool _RWSTD_COMP   = true;
    while ((void*)__x) {
        __y         = __x;
        _RWSTD_COMP = _C_key_comp(_KeyOf()(__v), __x->_C_key());
        __x         = _RWSTD_COMP ? __x->_C_left() : __x->_C_right();
    }
    if (_C_insert_always) {
      return _STD::pair<iterator, bool>(_C_insert(__x, __y, __v), true);
    }
    iterator __j = _C_make_iter(__y);   
    if (_RWSTD_COMP) {
      if (__j == begin())   {
        return _STD::pair<iterator, bool>(_C_insert(__x, __y, __v), true);
      }
      else
        --__j;
    }
    if (_C_key_comp (_ITER_NODE (__j)->_C_key (), _KeyOf ()(__v))) {
        return _STD::pair<iterator, bool>(_C_insert(__x, __y, __v), true);
    }
    return _STD::pair<iterator, bool>(__j, false);
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::iterator 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::insert (iterator __it,
                                                      const _Val& __v)
{
    _RWSTD_ASSERT_RANGE (begin (), __it);

    if (__it == begin ()) {
        if (   size() > 0
            && _C_key_comp (_KeyOf()(__v), _ITER_NODE (__it)->_C_key ()))
            return _C_insert (_ITER_NODE (__it), _ITER_NODE (__it), __v);
        return insert(__v).first;
    }
    else if (__it == end()) {
      if (_C_key_comp(_C_rightmost()->_C_key(), _KeyOf()(__v)))
        return _C_insert(0, _C_rightmost(), __v);
      else
        return insert(__v).first;
    }
    else {
        iterator __prev = --__it;
        if (   _C_key_comp (_ITER_NODE (__prev)->_C_key (), _KeyOf ()(__v))
            && _C_key_comp (_KeyOf()(__v), _ITER_NODE (__it)->_C_key ())) {
            if (!(void*)_ITER_NODE (__prev)->_C_right ())
                return _C_insert(0, _ITER_NODE (__prev), __v); 
            return _C_insert (_ITER_NODE (__it), _ITER_NODE (__it), __v);
        }
        return insert (__v).first;
    }
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::iterator 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::erase (iterator __it)
{
    _RWSTD_ASSERT_RANGE (begin (), __it);

    iterator __tmp = __it; 

    if (__tmp == end())
        // Attempting to erase an empty container.
        return end ();
    else
        // Retain 'next' __it for return.
        ++__tmp;


    _C_link_t __z (_ITER_NODE (__it));
    _C_link_t __y = __z;
    _C_link_t __x;
    bool __deleted = false;
    if (!(void*)__y->_C_left()) {
      if (!(void*)__y->_C_right()) {
        __x = __y->_C_parent();
        _C_erase_leaf(__y);
        __deleted = true;
      }
      else
        __x = __y->_C_right(); 
    }
    else {
      if (!(void*)__y->_C_right())
        __x = __y->_C_left();
      else {
        __y = __y->_C_right();
        while ((void*)__y->_C_left())
            __y = __y->_C_left();
        __x = __y->_C_right();
      }
    }
    if (!__deleted && __y != __z) {
      //
      // Relink __y in place of __z.
      //
      __z->_C_left()->_C_parent() = __y; 
      __y->_C_left() = __z->_C_left();
      if (__y != __z->_C_right()) {
        if ((void*)__x)
          __x->_C_parent()        = __y->_C_parent();
        __y->_C_parent()->_C_left()  = __x;         // __y must be a left child.
        __y->_C_right()         = __z->_C_right();
        __z->_C_right()->_C_parent() = __y;
      }
      else if ((void*)__x)
        __x->_C_parent() = __y;  

      if (_C_root() == __z)
        _C_root() = __y;
      else if (__z->_C_parent()->_C_left() == __z)
        __z->_C_parent()->_C_left() = __y;
      else 
        __z->_C_parent()->_C_right() = __y;

      __y->_C_parent() = __z->_C_parent();
      if (!(void*)__x)
        __x = __y;       // Don't want to start balancing with nil

      _STD::swap(__y->_C_color(), __z->_C_color());

      __y = __z;        // __y points to node to be actually deleted.
    }
    else if (!__deleted) {
      //
      // __y == __z
      //
      __x->_C_parent() = __y->_C_parent();   
      if (_C_root() == __z)
        _C_root() = __x;
      else  {
        if (__z->_C_parent()->_C_left() == __z)
          __z->_C_parent()->_C_left() = __x;
        else
          __z->_C_parent()->_C_right() = __x;
      }
      if (_C_leftmost() == __z)  {
        if (!(void*)__z->_C_right())  // __z->_C_left() must be NIL also.
          _C_leftmost() = __z->_C_parent();
        else
          _C_leftmost() = _C_node_t::_C_minimum(__x);
      }
      if (_C_rightmost() == __z)   {
        if (!(void*)__z->_C_left()) // __z->_C_right() must be NIL also.
          _C_rightmost() = __z->_C_parent();
        else
          _C_rightmost() = _C_node_t::_C_maximum(__x);
      }
    }
    if (__x != _C_header && __y->_C_color() != _C_node_t::_C_red) {
      while (__x != _C_root() && __x->_C_color() == _C_node_t::_C_black) {
        if (__x == __x->_C_parent()->_C_left()) {
          _C_link_t __w = __x->_C_parent()->_C_right();
          if (!(void*)__w) {
            __x->_C_color() = _C_node_t::_C_red;
            __x = __x->_C_parent();
          }
          else {
            if (__w->_C_color() == _C_node_t::_C_red) {
              __w->_C_color()         = _C_node_t::_C_black;
              __x->_C_parent()->_C_color() = _C_node_t::_C_red;
              __rotate_left(__x->_C_parent());
              __w = __x->_C_parent()->_C_right();
            }
            if (!(void*)__w) {
              __x->_C_color() = _C_node_t::_C_red;
              __x = __x->_C_parent();
            }
            else if ((   !(void*)__w->_C_left()
                      || __w->_C_left()->_C_color() == _C_node_t::_C_black) && 
                     (  !(void*)__w->_C_right()
                      || __w->_C_right()->_C_color() == _C_node_t::_C_black)) {
              __w->_C_color() = _C_node_t::_C_red;
              __x = __x->_C_parent();
            }
            else {
              if (   !(void*)__w->_C_right()
                  || __w->_C_right()->_C_color() == _C_node_t::_C_black) {
                if ((void*)__w->_C_left())
                  __w->_C_left()->_C_color() = _C_node_t::_C_black;
                __w->_C_color()       = _C_node_t::_C_red;
                __rotate_right(__w);
                __w = __x->_C_parent()->_C_right();
              }
              if ((void*)__w) {
                __w->_C_color() = __x->_C_parent()->_C_color();
                __x->_C_parent()->_C_color() = _C_node_t::_C_black;
                if ((void*)__w->_C_right())
                  __w->_C_right()->_C_color()  = _C_node_t::_C_black;
                __rotate_left(__x->_C_parent());
              }
              break;
            }
          }
        }
        else {
          //
          // Same as then clause with "right" and "left" exchanged.
          //
          _C_link_t __w = __x->_C_parent()->_C_left();
          if (!(void*)__w) {
            __x->_C_color() = _C_node_t::_C_red;
            __x = __x->_C_parent();
          }
          else {
            if (__w->_C_color() == _C_node_t::_C_red) {
              __w->_C_color()         = _C_node_t::_C_black;
              __x->_C_parent()->_C_color() = _C_node_t::_C_red;
              __rotate_right(__x->_C_parent());
              __w = __x->_C_parent()->_C_left();
            }
            if (!(void*)__w) {
              __x->_C_color() = _C_node_t::_C_red;
              __x = __x->_C_parent();
            }
            else if (   (   !(void*)__w->_C_right()
                         || __w->_C_right()->_C_color() == _C_node_t::_C_black)
                     && (   !(void*)__w->_C_left()
                         || __w->_C_left()->_C_color() == _C_node_t::_C_black)) {
              __w->_C_color() = _C_node_t::_C_red; __x = __x->_C_parent();
            }
            else {
              if (   !(void*)__w->_C_left()
                  || __w->_C_left()->_C_color() == _C_node_t::_C_black) {
                if ((void*)__w->_C_right())
                  __w->_C_right()->_C_color() = _C_node_t::_C_black;
                __w->_C_color()        = _C_node_t::_C_red;
                __rotate_left(__w);
                __w = __x->_C_parent()->_C_left();
              }
              if ((void*)__w) {
                __w->_C_color() = __x->_C_parent()->_C_color();
                __x->_C_parent()->_C_color() = _C_node_t::_C_black;
                if ((void*)__w->_C_left())
                  __w->_C_left()->_C_color()   = _C_node_t::_C_black;
                __rotate_right(__x->_C_parent());
              }
              break;
            }
          }
        }
      }
      __x->_C_color() = _C_node_t::_C_black;
    }
    _C_put_node(__y);
    --_C_node_count;
    return __tmp;
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::size_type 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::erase (const _Key& __x)
{
    _STD::pair<iterator, iterator> __p = equal_range(__x);
    size_type __n = _DISTANCE(__p.first, __p.second, size_type);
    erase(__p.first, __p.second);
    return __n;
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::_C_link_t 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::
_C_copy (_C_link_t __x, _C_link_t __p)
{
    // Structural copy.
    _C_link_t __res = __x;

    while ((void*)__x) {
      _C_link_t __y = _C_get_node(__x->_C_value());
      if (__res == __x)
          __res = __y;  // Save for return value.

      __p->_C_left()   = __y;
      __y->_C_parent() = __p;
      __y->_C_color()  = __x->_C_color();
      __y->_C_right()  = _C_copy(__x->_C_right(), __y);
      __p              = __y;
      __x              = __x->_C_left();
    }
    __p->_C_left() = 0;
    return __res;
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
void __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::_C_erase (_C_link_t __x)
{
    //
    // Erase without rebalancing.
    //
    while ((void*)__x) {
        _C_erase(__x->_C_right());
        _C_link_t __y = __x->_C_left();
        _C_put_node(__x);
        __x = __y;
    }
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::iterator 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::erase (iterator __first, 
                                                     iterator __last)
{
    _RWSTD_ASSERT_RANGE (begin (), __first);
    _RWSTD_ASSERT_RANGE (__first, __last);

    iterator __tmp = end();
    if (__first == begin() && __last == end() && _C_node_count != 0) {
      _C_erase(_C_root());
      _C_leftmost()  = _C_header;
      _C_root()      = 0;
      _C_rightmost() = _C_header;
      _C_node_count  = 0;
      __tmp = end();
    } else {
        while (__first != __last)
            __tmp =  erase(__first++);
    }
    return __tmp;
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::iterator 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::find (const _Key& __k)
{
    _C_link_t __y = _C_header;  // __last node that is not less than __k.
    _C_link_t __x = _C_root();  // Current node.

    while ((void*)__x) {
      if (!_C_key_comp(__x->_C_key(), __k))
        __y = __x, __x = __x->_C_left();
      else
        __x = __x->_C_right();
    }

    iterator __j = _C_make_iter (__y);
    return (   __j == end()
            || _C_key_comp (__k, _ITER_NODE (__j)->_C_key ())) ? end () : __j;
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::iterator 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::lower_bound (const _Key& __k)
{
    _C_link_t __y = _C_header;  // __last node that is not less than __k.
    _C_link_t __x = _C_root();  // Current node.

    while ((void*)__x) {
      if (!_C_key_comp(__x->_C_key(), __k))
        __y = __x, __x = __x->_C_left();
      else
        __x = __x->_C_right();
    }

    return _C_make_iter (__y);
}


template <class _Key, class _Val, class _KeyOf, class _Comp, class _Alloc>
_TYPENAME __rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::iterator 
__rb_tree<_Key, _Val, _KeyOf, _Comp, _Alloc>::upper_bound (const _Key& __k)
{
    _C_link_t __y = _C_header;  // __last node that is greater than __k.
    _C_link_t __x = _C_root();  // Current node.

    while ((void*)__x) {
      if (_C_key_comp(__k,__x->_C_key()))
        __y = __x, __x = __x->_C_left();
      else
        __x = __x->_C_right();
    }

    return _C_make_iter (__y);
}


_RWSTD_NAMESPACE_END   // __rw


#undef _ITER_NODE
