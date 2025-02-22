
#include "helper_day11part2.hpp"

int main(int argc, char** argv)
{
    int blink_count = 75;

    std::string file_p="puzzle_input.txt";
    //auto file_p="sample_2.txt";


    // if user input given then use those
    bool is_given = true;
    int temp_blnk;
    std::string  temp_file;

    if(argc==3)
    {
        try
        {
            temp_blnk = std::stoi(argv[1]);
        }
        catch(...)
        {
            is_given = false;
            std::cout << "\nBad input for blink count\n";
        }

        try
        {
            temp_file = argv[2];
        }
        catch(...)
        {
            is_given = false;
            std::cout << "\nBad input for file path\n";
        }
    }

    if(is_given)
    {
        blink_count = temp_blnk;
        file_p = temp_file;
    }

    STRLST numstr_list;
    if(!get_num_str_list(file_p, numstr_list))
    {
        std::cout << "\n Failed to parse the lines in file: \"" << file_p << "\"\n";
    }

    std::cout << "\nGiven string:";
    for(const auto s : numstr_list)
    {
        std::cout << s << " " ;
    }
    std::cout << "\n";

    auto start_time = std::chrono::high_resolution_clock::now();

    bool stonesdone=false;
    int curr_idx=1;
    try
    {
        perform_stone_blinks(blink_count, numstr_list, curr_idx);
    }
    catch(...)
    {
        std::cout << "\n Failed at idx \"" << curr_idx << " \" !!!\n\n";
    }

    if(!stonesdone)
    {
        constexpr bool dump_it = true;
        print_stones(numstr_list, curr_idx, dump_it);
    }

    /*try
    {
        // print the solution
        print_stones(numstr_list, blink_count);
    }
    catch(...)
    {
        std::cout << "\n Failed to print the stones of size \""
                  << numstr_list.size() << " \" after "
                  << blink_count << " blinks!!!\n\n";
    }*/
    std::cout << "\n Size of the stones \"" << numstr_list.size() << "\"\n";

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    auto duration__s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

    bool is_sec = duration__s>0;
    auto duration = (is_sec ? duration__s : duration_ms);

    std::cout << "\nTime taken: " << duration << (is_sec ? " s" : " ms") << "\n";

    return 0;
}
