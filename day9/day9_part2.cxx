#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<chrono>
#include<memory>
#include<list>

using NUMTYPE=unsigned long long;
using STRTYPE=std::string;
using STRVECTYPE=std::vector<std::string>;
using BLOCKTYPE=std::string;
using BLOCKLIST=std::list<BLOCKTYPE>;

int get_int_from_char(const char& ch)
{
    return std::stoi(std::string({ch}));
}
const NUMTYPE get_num_from_str(const BLOCKTYPE& str)
{
    return std::stoull(str);
}
BLOCKTYPE create_block(int idata)
{
    return std::to_string(idata);
}
BLOCKTYPE create_block(const char cdata)
{
    return std::string({cdata});
}

struct BLOCKSTREAM
{
    BLOCKLIST _blockstream;
    NUMTYPE _index=-1;
    bool _is_done = false;

    // CTOR
    BLOCKSTREAM(const int rank):_index(rank){}

    // copy-CTOR
    BLOCKSTREAM(const BLOCKSTREAM& strm)
    {
        this->reset();
        _index=strm.view_index();
        for(const auto& b : strm.view_blockstream())
            this->_blockstream.emplace_back(b);
        this->_is_done=(strm.is_done())?true:false;
    };

    // helper functions
    void reset()
    {
        _index=-1;
        _blockstream.clear();
        _is_done=false;
    }
    bool is_invalid() const
    {
        return _index<=-1 || _blockstream.empty();
    }
    bool is_valid() const
    {
        return false==is_invalid();
    }
    bool is_space(const BLOCKTYPE& space_symbol) const
    {
        if(_blockstream.empty())
            throw;
        return _blockstream.front()==space_symbol;
    }
    bool is_space(const char& space_symbol) const
    {
        if(_blockstream.empty())
        {
            std::cout << "\nBAD BAD .. FOUND EMPTY STREAM!!!!";
            throw;
        }
        return _blockstream.front()[0]==space_symbol;
    }

    // setter functions
    void set_blockstream(const BLOCKLIST& blockstream)
    {
        this->_blockstream.clear();
        for(const BLOCKTYPE& b : blockstream)
            this->_blockstream.emplace_back(b);
    }
    void set_blockstream(const BLOCKTYPE& blockdata, const NUMTYPE& freq)
    {
        this->_blockstream.clear();
        for(NUMTYPE i=0; i<freq; ++i)
            this->_blockstream.emplace_back(blockdata);
    }
    void set_index(const int index)
    {
        this->_index = index;
    }
    void set_is_done()
    {
        this->_is_done = true;
    }

    // viewer/getter functions
    bool is_done() const { return _is_done;}
    const int view_index() const {return this->_index;}
    const BLOCKLIST& view_blockstream() const {return this->_blockstream;}
    BLOCKLIST& get_blockstream(){return this->_blockstream;}

    // operators
    // equal
    BLOCKSTREAM& operator=(const BLOCKSTREAM& blkstrm)
    {
        this->reset();
        this->set_index(blkstrm.view_index());
        this->set_blockstream(blkstrm.view_blockstream());
        this->_is_done = (blkstrm.is_done()) ? true : false;
        return *this;
    }
    // add then assign
    BLOCKSTREAM& operator+=(const BLOCKTYPE& b)
    {
        this->_blockstream.emplace_back(b);
        return *this;
    }
    // outstream operator
    friend std::ostream& operator<<(std::ostream& ostrm, const BLOCKSTREAM& blkstrm)
    {
        for(const auto& blk : blkstrm.view_blockstream())
            ostrm << blk;
        return ostrm;
    }

    void populate_blockstream(const BLOCKTYPE& b_data, const int b_freq)
    {
        for(int i_block=0; i_block<b_freq; ++i_block)
        {
            *this += b_data;
        }
    }
};
using BLOCKSTREAMLIST=std::list<BLOCKSTREAM>;


void generate_string(const BLOCKSTREAMLIST& blkstrmlist, STRTYPE& str1)
{
    for(const auto& blkstrm : blkstrmlist)
        for(const auto& b : blkstrm.view_blockstream())
            str1 += b;
}


const NUMTYPE get_checksum(const char sapce_symbol, const BLOCKSTREAMLIST& streamlist)
{
    NUMTYPE cks=0;
    NUMTYPE idx=0;
    for(const BLOCKSTREAM& strm : streamlist)
    {
        if(!strm.is_space(sapce_symbol))
        {
            //std::cout << "\n Generating check-sum for the block: " << strm;
            for(const BLOCKTYPE& s : strm.view_blockstream())
            {
                NUMTYPE i_add = get_num_from_str(s);
                //std::cout << "\n\tAdding  " << i_add << "*" << idx;

                i_add = idx*i_add;
                //std::cout  << "  ( or " << i_add << ") ";

                cks = cks + i_add;
                //std::cout << " and total checksum = " << cks;

                idx = idx + 1;
            }
            //std::cout << " and total checksum = " << cks;
        }
        else
        {
            //std::cout << "\n Skipping the check-sum process for the block: " << strm << "\n";
            idx = idx + strm.view_blockstream().size();
        }
    }
    return cks;
}


void extract_blokstream_list(const char space_symbol, const std::string& fix_str, BLOCKSTREAMLIST& blkstrmlst)
{
    bool is_file=true;

    int file_id=0;
    int stream_rank=0;


    for(const char& c : fix_str)
    {
        int ifreq = get_int_from_char(c);

        BLOCKSTREAM blocks(stream_rank);

        if(ifreq==0)
        {
            //std::cout<<"\tNo block to be entered!!!\n";
            ++stream_rank;
            is_file=!is_file; // toggle flag
            continue;
        }

        if(is_file)
        {
            //std::cout << "\tAdding \"" << file_id << "\" " << ifreq << " times \n";
            blocks.set_blockstream(create_block(file_id), ifreq);
            ++file_id;
        }
        else
        {
            //std::cout << "\tAdding \"" << space_symbol << "\" " << ifreq << " times \n";
            blocks.set_blockstream(create_block(space_symbol), ifreq);
        }

        blkstrmlst.emplace_back(blocks);

        ++stream_rank;
        is_file=!is_file; // toggle flag
    }
}


void sort_blokstream_list(const char space_symbol, BLOCKSTREAMLIST& blkstrmlst)
{
    std::cout << "\n\n.. Starting the sorting process of block streams list ...\n\n";

    NUMTYPE n_space=0;
    NUMTYPE n_files=0;

    std::cout << "..... counting number of blocks ..... \n";
    for(const BLOCKSTREAM& blkstrm : blkstrmlst)
    {
        if(blkstrm.is_space(space_symbol))
        {
            ++n_space;
            //std::cout << "\nFound a spaces block: " << blkstrm;
        }
        else
        {
            ++n_files;
            //std::cout << "\nFound a fileID block: " << blkstrm;
        }
    }

    const NUMTYPE n_total = n_files + n_space;

    std::cout << "\n Total blocks of spaces: " << n_space;
    std::cout << "\n Total blocks of flieID: " << n_files;
    std::cout << "\n Total blocks : " << n_total << "\n";;


    auto itr_blocks=std::prev(blkstrmlst.end()); // start from the end of the vector

    std::cout << "\n Starting sort .... " << "\n";
    for(; itr_blocks !=blkstrmlst.begin(); --itr_blocks)
    {
        if(!itr_blocks->is_space(space_symbol) && !itr_blocks->is_done())
        {
            //std::cout << "\nTrying to swap \""<< *itr_blocks << "\" of rank [" << itr_blocks->view_index() <<"] ";

            auto itr_spaces = std::find_if(blkstrmlst.begin(),
                                          blkstrmlst.end(),
                                          [&space_symbol, &itr_blocks](const BLOCKSTREAM& strm)->bool
                                          {
                                              return strm.is_space(space_symbol) && 
                                                     strm.view_blockstream().size()>=itr_blocks->view_blockstream().size();
                                          });

            if(itr_spaces==blkstrmlst.end() || itr_spaces->view_index() > itr_blocks->view_index())
            {
                //std::cout << "; No suitable spaces-block found to replace it!";
                continue;
            }

            //std::string gen;
            //std::cout << "; swapping with \"" << *itr_spaces << "\' of rank [" << itr_spaces->view_index() << "]";

            // assert if spaces-bloc size found is smaller than that of files-ID block
            const auto n_spc_size = itr_spaces->view_blockstream().size();
            const auto n_blk_size = itr_blocks->view_blockstream().size();
            if(n_spc_size < n_blk_size)
            {
                std::cout << "\n FOUND SPACES BLOCKS IS SMALLER THAN FILES-ID BLOCK !!!!\n\n";
                throw;
            }

            // do speacial swap : file-id-blocks with spaces-blocks
            BLOCKSTREAM tmp = *itr_spaces; // create tmp as back-up
            *itr_spaces = *itr_blocks; // allocate file-id blocks to spaces blocks in list
            itr_spaces->set_is_done();
            itr_blocks->set_blockstream(create_block(space_symbol), n_blk_size); // fill up the file-id block with spaces now
            itr_blocks->set_is_done();

            // insert a new block if spaces-block size was bigger than file-id block size;
            if(n_spc_size > n_blk_size)
            {
                // fill tmp with spaces as many file-id block-data
                tmp.set_blockstream(create_block(space_symbol), n_spc_size - n_blk_size);
                 // iterator pointint to the spot where the insertion will be done
                auto iterx = std::next(itr_spaces);
                // insert just after the currently processed spaces-block
                blkstrmlst.insert(iterx, tmp);
                // fix the ranking
                auto newindex=0;
                for(iterx = blkstrmlst.begin(); iterx != blkstrmlst.end(); ++iterx)
                {
                    iterx->set_index(newindex++);
                }
            }
            // print result
            //generate_string(blkstrmlst, gen);
            //std::cout << "; result: " << gen;
        }
    }
}


bool get_file_line(const std::string& file_path, std::string& line)
{
    std::ifstream iFile(file_path);

    if(!iFile.is_open())
        return false;

    std::string str;
    while(iFile>>str)
    {
        line += str;
    }
    return true;
}


int main(int argc, char** argv)
{
    const auto t0 = std::chrono::high_resolution_clock::now();

    const std::string pFile="puzzle_input.txt";
    //const std::string pFile="sample_input.txt";

    std::string input_line;
    if(!get_file_line(pFile, input_line))
    {
        std::cout<<"\nFailed to read from file : " + pFile << "\n";
        return -1;
    }

    constexpr char space_symbol = '.';

    BLOCKSTREAMLIST blk_streams;
    extract_blokstream_list(space_symbol, input_line, blk_streams);
    std::cout << "After extracting files and spaces: \"" ;
    STRTYPE blockstreamstring;
    generate_string(blk_streams, blockstreamstring);
    std::cout << blockstreamstring << "\"\n";

    const auto timeTaken1 =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t0).count();
    std::cout << "Time taken: " << timeTaken1 << " ms";//<< " microseconds \n";

    sort_blokstream_list(space_symbol, blk_streams);
    std::cout << "After sorting files and spaces: \"" ;
    blockstreamstring.clear();
    generate_string(blk_streams, blockstreamstring);
    std::cout << blockstreamstring << "\"\n";

    std::cout << "\n\n Now generating the check-sum .....\n\n\n";
    const auto chksum = get_checksum(space_symbol, blk_streams);
    std::cout<<"\n check-sum from string: " << chksum << "\n";

    const auto timeTaken2 =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t0).count();
    std::cout << "\n\n Time taken: " << timeTaken2 << " ms \n";//<< " microseconds \n";

    return 0;
}

