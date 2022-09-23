#ifndef _YHSTL_ITERATOR_H_
#define _YHSTL_ITERATOR_H_
#include<cstddef>      //�������ͷ�ļ�����Ҫ���õ�   _WIN64 typedef __int64          ptrdiff_t;��Щ��������
#include "type_traits.h"   //����Ҫʵ�� type_traits.h

//STL����˼������������㷨�ֿ���������ƣ����������������ߵ�ճ�ϼ�
//20220921 ��������һ������ָ�룬����������������
//20220922 �����Ҫʵ����advanceʱ����װһ�£�����_advance����������iterator_category,���ݲ�ͬ��������ȡ��ͬ����
namespace yhstl
{   
    //������Ƕ�ͱ���
	struct input_iterator_tag {};      
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	// iterator ģ��  �κ��¶���ĵ�������ü̳�����������ں�������������Ĭ��ֵ������ֻҪ����ǰ������������
	template <class Category, class T, class Distance = ptrdiff_t,class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef Category                             iterator_category;
		typedef T                                    value_type;
		typedef Pointer                              pointer;
		typedef Reference                            reference;
		typedef Distance                             difference_type;
	};

    // iterator traits
    //ե֭��  traits
    template <class Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
        typedef typename Iterator::difference_type   difference_type;
    };

    // ���ԭ��ָ���ƫ�ػ��汾
    template <class T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator_tag           iterator_category;
        typedef T                                    value_type;
        typedef T*                                   pointer;
        typedef T&                                   reference;
        typedef ptrdiff_t                            difference_type;
    };
    //���pointer-to-const ��Ƶ�traitsƫ�ػ���
    template <class T>
    struct iterator_traits<const T*>
    {
        typedef random_access_iterator_tag           iterator_category;
        typedef T                                    value_type;
        typedef const T*                             pointer;
        typedef const T&                             reference;
        typedef ptrdiff_t                            difference_type;
    };


    // ��ȡĳ���������� category
    template <class Iterator>
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category();
    }

    // ��ȡĳ���������� distance_type
    template <class Iterator>
    typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    // ��ȡĳ���������� value_type
    template <class Iterator>
    typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    // ���º������ڼ����������ľ���
    //��������� ͨ������ʵ�� advance����
    // distance �� input_iterator_tag �İ汾
    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last)
        {
            ++first;
            ++n;
        }
        return n;
    }

    // distance �� random_access_iterator_tag �İ汾
    template <class RandomIter>
    typename iterator_traits<RandomIter>::difference_type
    distance_dispatch(RandomIter first, RandomIter last,
            random_access_iterator_tag)
    {
        return last - first;
    }

    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last)
    {
        return distance_dispatch(first, last, iterator_category(first));
    }

    // ���º��������õ�����ǰ�� n ������

    // advance �� input_iterator_tag �İ汾
    template <class InputIterator, class Distance>
    void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
    {
        while (n--)
            ++i;
    }

    // advance �� bidirectional_iterator_tag �İ汾
    template <class BidirectionalIterator, class Distance>
    void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
    {
        if (n >= 0)
            while (n--)  ++i;
        else
            while (n++)  --i;
    }

    // advance �� random_access_iterator_tag �İ汾
    template <class RandomIter, class Distance>
    void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag)
    {
        i += n;
    }

    template <class InputIterator, class Distance>
    void advance(InputIterator& i, Distance n)
    {
        advance_dispatch(i, n, iterator_category(i));
    }

    // ģ���� : reverse_iterator
    // ���������������ʹǰ��Ϊ���ˣ�����Ϊǰ��
    template <class Iterator>
    class reverse_iterator
    {
    private:
        Iterator current;  // ��¼��Ӧ�����������

    public:
        // �����������������Ӧ�ͱ�
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type        value_type;
        typedef typename iterator_traits<Iterator>::difference_type   difference_type;
        typedef typename iterator_traits<Iterator>::pointer           pointer;
        typedef typename iterator_traits<Iterator>::reference         reference;

        typedef Iterator                                              iterator_type;
        typedef reverse_iterator<Iterator>                            self;

    public:
        // ���캯��
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type i) :current(i) {}
        reverse_iterator(const self& rhs) :current(rhs.current) {}

    public:
        // ȡ����Ӧ�����������
        iterator_type base() const
        {
            return current;
        }

        // ���ز�����
        reference operator*() const
        { // ʵ�ʶ�Ӧ�����������ǰһ��λ��
            auto tmp = current;
            return *--tmp;
        }
        pointer operator->() const
        {
            return &(operator*());
        }

        // ǰ��(++)��Ϊ����(--)
        self& operator++()
        {
            --current;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            --current;
            return tmp;
        }
        // ����(--)��Ϊǰ��(++)
        self& operator--()
        {
            ++current;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            ++current;
            return tmp;
        }

        self& operator+=(difference_type n)
        {
            current -= n;
            return *this;
        }
        self operator+(difference_type n) const
        {
            return self(current - n);
        }
        self& operator-=(difference_type n)
        {
            current += n;
            return *this;
        }
        self operator-(difference_type n) const
        {
            return self(current + n);
        }

        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }
    };

    // ���� operator-
    template <class Iterator>
    typename reverse_iterator<Iterator>::difference_type
        operator-(const reverse_iterator<Iterator>& lhs,
            const reverse_iterator<Iterator>& rhs)
    {
        return rhs.base() - lhs.base();
    }

    // ���رȽϲ�����
    template <class Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs,
        const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <class Iterator>
    bool operator<(const reverse_iterator<Iterator>& lhs,
        const reverse_iterator<Iterator>& rhs)
    {
        return rhs.base() < lhs.base();
    }

    template <class Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs,
        const reverse_iterator<Iterator>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs,
        const reverse_iterator<Iterator>& rhs)
    {
        return rhs < lhs;
    }

    template <class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs,
        const reverse_iterator<Iterator>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs,
        const reverse_iterator<Iterator>& rhs)
    {
        return !(lhs < rhs);
    }
}
#endif