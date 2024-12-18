#include<iostream>


#include "../get_from_file.cpp"
#include "../day_2_report_safety_analyser.cpp"

std::string strPath = "../day_2_report.txt";


bool try_w_pos(const int pos, const std::vector<int>& vec_in)
{
    int notused;
    std::vector<int> vec_out = vec_in;
    vec_out.erase(vec_out.begin() + pos);
    return  day_2_report_safety_handler(vec_out, notused);
}


int main()
{
    int n_safe_count = 0;
    std::list<std::vector<int>> report_data;

    if(get_vector_from_file<int>(strPath, report_data))
    {
        for(const auto vec : report_data)
        {
            int pos = -9999;
            if(day_2_report_safety_handler(vec, pos))
            {
                ++n_safe_count;
            }
            else if(pos > 0)
            {
                bool new_safe = false;
                /*int new_pos = pos;
                // try thrice with new vectors having following 
                // indices removed in each case:
                // case-1: pos
                // case-2: pos -1
                // case-3: pos -2
                while(new_pos)
                {
                    new_safe = new_safe || try_w_pos(new_pos, vec);
                    --new_pos;
                }*/

                for(int s = 0; s < vec.size(); ++s)
                {
                    new_safe = new_safe || try_w_pos(s, vec);
                }
                
                if(new_safe)
                {
                    print_vec(vec);
                    ++n_safe_count;
                }
            }
        }
    }

    std::cout << "\n Total safe reports: " << n_safe_count << "\n";
    //test_fetching_data_from_file();

    return 0;
}
