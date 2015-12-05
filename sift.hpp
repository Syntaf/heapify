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
void sift_down_range(RndIter first, RndIter last, Pred && pred, 
        typename std::iterator_traits<RndIter>::difference_type len,
        RndIter start, std::size_t count)
{
    for(int i = 0; i < count; i++) {
        //std::cout << "calling sift_down for " << start-i << std::endl;
        sift_down<RndIter>(first, last, std::forward<Pred>(pred), len, start - i);
    }
}
