#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

template<typename RndIter, typename Pred>
void sift_down(RndIter first, RndIter last, Pred && pred, 
        typename std::iterator_traits<RndIter>::difference_type len,
        RndIter start)
{
    typedef typename std::iterator_traits<RndIter>::difference_type difference_type;
    typedef typename std::iterator_traits<RndIter>::value_type value_type;

    //left-child of start is at 2 * start + 1
    //right-child of start is at 2 * start + 2
    
    difference_type child = start - first;

    if(len < 2 || (len - 2) / 2 < child)
        return;

    child = 2 * child + 1;
    RndIter child_i = first + child;

    if ((child + 1) < len && pred(*child_i, *(child_i+1))) {
        ++child_i;
        ++child;
    }

    if (pred(*child_i, *start))
        return;

    value_type top = *start;
    do {
        *start = *child_i;
        start = child_i;

        if ((len - 2) / 2 < child)
            break;

        child = 2 * child + 1;
        child_i = first + child;

        if ((child + 1) < len && pred(*child_i, *(child_i + 1))) {
            ++child_i;
            ++child;
        }
    }while(!pred(*child_i, top));
    *start = top;
}

template<typename RndIter, typename Pred>
void _make_heap(RndIter first, RndIter last, Pred && pred)
{
    typedef typename std::iterator_traits<RndIter>::difference_type 
        difference_type;

    // get distance between beginning and end of sequence
    difference_type n = last - first;

    std::cout << "Distance: " << n << std::endl;
    std::cout << "Height: " << (int)log2(n) << std::endl;
    if(n > 1) {
        // create heap
        for(difference_type start = (n-2)/2; 
            start > 0; 
            start = (difference_type)pow(2, (difference_type)log2(start)) - 2) {     
    
            difference_type end_level = (difference_type)pow(2, (difference_type)log2(start))-2;
            std::cout << start << " -to-> " << end_level << std::endl;
            difference_type p = (start-end_level) / 2; 

            std::cout << "p: " << p << " , end_level: " << end_level << std::endl;

            for(int i = 0; i < p; i++) {
                std::cout << "calling sift_down for " << start-i << std::endl;
                sift_down<RndIter>(first, last, std::forward<Pred>(pred), n, first + start - i);
            }
            for(int i = 0; i < p; i++) {
                std::cout << "calling sift_down for " << start-p-i << std::endl;
                sift_down<RndIter>(first, last, std::forward<Pred>(pred), n, first + start - p - i);
            }
        }

        sift_down<RndIter>(first, last, std::forward<Pred>(pred), n, first);

    }
}

int main() 
{
    std::vector<int> v(10); //= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::iota(v.begin(), v.end(), 0);

    std::cout << "Before: ";
    for(auto &c: v)
        std::cout << c << " ";
    std::cout << std::endl << std::endl;
    _make_heap(v.begin(), v.end(), std::less<int>());
    std::cout << std::endl;
    std::cout << "After: ";

    for(auto &c: v)
        std::cout << c << " ";
    std::cout << std::endl;
    
    bool worked = std::is_heap(v.begin(), v.end());

    std::cout << "is heap? " << (worked ? "true" : "false") << std::endl;

    return 0;
}
