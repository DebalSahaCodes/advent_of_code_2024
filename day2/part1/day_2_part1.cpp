#include<iostream>


#include "../get_from_file.cpp"
#include "../day_2_report_safety_analyser.cpp"


int main()
{
    int n_safe_count = 0;
    std::list<std::vector<int>> report_data;

    const std::string strPath = "../day_2_report.txt";
    if(get_vector_from_file<int>(strPath, report_data))
    {
        for(const auto vec : report_data)
        {
            if(day_2_report_safety_analyser(vec))
                ++n_safe_count;
        }
    }

    std::cout << "\n Total safe reports: " << n_safe_count << "\n";
    //test_fetching_data_from_file();

    return 0;
}
