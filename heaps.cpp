#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/util/high_resolution_clock.hpp>
#include <hpx/include/async.hpp>
#include <hpx/util/lightweight_test.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

#include "sift.hpp"


template<typename RndIter, typename Pred>
void _make_heap(RndIter first, RndIter last, Pred && pred, std::size_t chunk_size)
{
    typedef typename std::iterator_traits<RndIter>::difference_type 
        difference_type;

    // get distance between beginning and end of sequence
    difference_type n = last - first;

    if(n > 1) {
        // Starting at location (n-2)/2, iterate up the RIGHT MOST element of each level.
        // This loop is the barrier for parallelism within this algorithm, each level must
        // synchronoize before the next level can begin at it's right most elements. Each
        // subsequent value of start can be determined by 2^(log2(start)) - 2
        for(difference_type start = (n-2)/2; start > 0; 
            start = (difference_type)pow(2, (difference_type)log2(start)) - 2) {     
    
            // Find the location of the next level, elements between start and end will be
            // partitioned and executed in parallel
            difference_type end_level = (difference_type)pow(2, (difference_type)log2(start))-2;

            std::cout << start << " - " << end_level << std::endl;
            // range is [start,end_level) , thus leave one off when counting items
            std::size_t items = (start-end_level);

            if(chunk_size > items)
                chunk_size = items / 2;

            std::vector<hpx::future<void> > workitems;
            workitems.reserve(items/chunk_size);
            std::size_t cnt = 0;
            std::cout << "Items: " << items << ", Chunk_size: " << chunk_size << std::endl;
            while(cnt+chunk_size < items) {
                std::cout << "Launching items " << start - cnt << " to " 
                          << start - cnt - chunk_size << std::endl;
                workitems.push_back(
                    hpx::async(hpx::launch::async,
                               &sift_down_range<RndIter, Pred>, first, last,
                               std::forward<Pred>(pred), n, first + start - cnt,
                               chunk_size)
                    );
                cnt += chunk_size;
            }

            if(cnt < items) {
                std::cout << "lLaunching items " << start - cnt << " to " 
                          << start - cnt - (items-cnt) << std::endl;
                workitems.push_back(
                    hpx::async(hpx::launch::async,
                               &sift_down_range<RndIter, Pred>, first, last,
                               std::forward<Pred>(pred), n, first + start - cnt,
                               items-cnt)
                    );
            }

            hpx::wait_all(workitems);
        }

        // Perform sift down for the head node
        sift_down<RndIter>(first, last, std::forward<Pred>(pred), n, first);

    }
}

int hpx_main(boost::program_options::variables_map& vm)
{
    std::size_t chunk_size = vm["chunk_size"].as<std::size_t>();
    std::size_t vector_size = vm["vector_size"].as<std::size_t>();
    std::vector<int> v(vector_size);
    std::iota(v.begin(), v.end(), 0);

    std::cout << "Distance of tree: " << vector_size << std::endl;
    std::cout << "Height: " << (int)log2(vector_size) << std::endl;

    std::cout << "Running parallel make heap...\n";
    boost::uint64_t start = hpx::util::high_resolution_clock::now();
    _make_heap(v.begin(), v.end(), std::less<int>(), chunk_size);
    boost::uint64_t time = hpx::util::high_resolution_clock::now() - start;
    bool worked = std::is_heap(v.begin(), v.end());

    HPX_TEST_EQ(worked, true);

    std::vector<int> v2(vector_size);
    std::iota(v2.begin(), v2.end(), 0);

    std::cout << "Running sequential make heap...\n";
    boost::uint64_t start1 = hpx::util::high_resolution_clock::now();
    std::make_heap(v2.begin(), v2.end(), std::less<int>());
    boost::uint64_t time2 = hpx::util::high_resolution_clock::now() - start1;
    bool worked2 = std::is_heap(v2.begin(), v2.end());

    HPX_TEST_EQ(worked2, true);

    std::cout << "---------------------------------------------\n";

    std::cout << "Results:\nParallel: " << time/1e9 << "\nSequential: " << time2/1e9 
              << std::endl;

    return hpx::finalize();
}

int main(int argc, char* argv[]) 
{
    // configure application-specific options
    boost::program_options::options_description 
        cmdline("usage: " HPX_APPLICATION_STRING " [options]");

    cmdline.add_options()
        ("vector_size"
        , boost::program_options::value<std::size_t>()->default_value(25)
        , "size of vector")
        
        ("chunk_size"
        , boost::program_options::value<std::size_t>()->default_value(2)
        , "amount of work per thread")
        ;

    return hpx::init(cmdline, argc, argv);
}
