#include<iostream>
#include<fstream>
#include<vector>
#include<string>

struct Array2D {

  // A 2D grid is the model assumed
  // Each character is a point in grid
  // with (x,y) representing the coordinates
  // and (0,0) at top-left
  //
  std::vector<std::string> _grid;

  // CTOR
  Array2D() = delete;
  Array2D(const std::vector<std::string>& inG)
  {
    for(const auto s : inG)
    {
      add_line(s);
    }
  }

  const char get_char_at_xy(int x, int y) const
  {
    if(x>=0 && x<= get_XMAX() && y>=0 && y<= get_YMAX())
    {
      //std::cout << "pos " << x << "," << y << "\n";
      return _grid[y][x];
    }
    else
    { 
      std::cout << "Bad position or coordinate;Throw\n";
      throw;
    }
  }

  void add_line(const std::string& str)
  {
    _grid.emplace_back(str);
  }

  int get_XMAX() const
  {
    return _grid[0].size() - 1;
  }

  int get_YMAX() const
  {
    return _grid.size() - 1;
  }

  void get_all_horz(std::vector<std::string>& strVec) const
  {
    int i = 0;
    for (const auto s : _grid)
    {
      //std::cout << "\n Adding: \"" + s + "\"";
      strVec.emplace_back(s);
    }
  }

  void get_all_vert(std::vector<std::string>& strVec) const // read each char in first col, then 2nd col etc.
  {
    int x = 0;
    while(x<=get_XMAX())
    {
      std::string str;
      for(int y=0; y<= get_YMAX(); ++y)
      {
        str += get_char_at_xy(x,y);
      }
      //std::cout << "Vert : " << str << "\n";
      strVec.emplace_back(str);
      ++x;
    }
  }

  void get_all_diag_LT2RB(std::vector<std::string>& strVec) const // Left-TOP to Right-Bottom
  {
    const int XMX = get_XMAX();
    int offset = 0; //for upper right half
    while(offset <= XMX)
    {
      std::string str;
      for(int x=offset, y=0; x<=XMX && y<=get_YMAX(); ++x,++y)
      {
        str += get_char_at_xy(x,y);
      }
      //std::cout << "Diag : " << str << "\n";
      strVec.emplace_back(str);
      ++offset;
    }
    offset = 1; //for lower left half (0 already done)
    while(offset <= XMX)
    {
      std::string str;
      for(int x=0, y=offset; x<=XMX && y<=get_YMAX(); ++x,++y)
      {
        str += get_char_at_xy(x,y);
      }
      //std::cout << "Diag : " << str << "\n";
      strVec.emplace_back(str);
      ++offset;
    }
  }

  void get_all_diag_RT2BL(std::vector<std::string>& strVec) const // right-TOP to Bottom-left
  {
    const int XMX = get_XMAX();
    int offset = XMX; //lower right
    while(offset>=0)
    {
      std::string str;
      for(int x=offset, y=0; x>=0 && y<=get_YMAX(); --x,++y)
      {
        str += get_char_at_xy(x,y);
      }
      strVec.emplace_back(str);
      --offset;
    }
    offset = 1; //upper left (0 already done)
    while(offset <= XMX)
    {
      std::string str;
      for(int x=XMX, y=offset; x>=0 && y<=get_YMAX(); --x,++y)
      {
        str += get_char_at_xy(x,y);
      }
      //std::cout << "Diag : " << str << "\n";
      strVec.emplace_back(str);
      ++offset;
    }
  }

};

const std::string reverse_string(const std::string& s1)
{
  std::string str;
  for(auto i = s1.size(); i > 0; --i)
  {
    str += s1[i-1];
  }
  return str;
}

int count_xmas_in_vec(const std::vector<std::string>& sVec)
{
  int iCount = 0;
  std::cout << "\nSTART...\n";
  for(const auto st : sVec)
  {
    std::string cM = "";
    for(const char c : st)
    {
      cM += c;
      if(cM.size() >= 4 && "XMAS" == cM.substr(cM.size() - 4,4))
      {
        ++iCount;
        cM = "";
      }
    }
    //std::cout <<"Count str :" << iCount << "\n";
    const std::string sRev = reverse_string(st);
    cM = "";
    for(const char c : sRev)
    {
      cM += c;
      if(cM.size() >= 4 && "XMAS" == cM.substr(cM.size() - 4,4))
      {
        ++iCount;
        cM = "";
      }
    }
    //std::cout <<"Count rev str :" << iCount << "\n\n";
  }
  return iCount;
}

bool get_all_arrays(const Array2D& grd, std::vector<std::string>& all_str_vec)
{
  size_t sz = 0;

  grd.get_all_horz(all_str_vec);
  sz = all_str_vec.size();
  std::cout << sz << "\n";
  for(const auto s : all_str_vec)
  {
    std::cout << s << "\n";
  }
  std::cout << "xmas count: " << count_xmas_in_vec(all_str_vec) << "\n";

  grd.get_all_vert(all_str_vec);
  std::cout << all_str_vec.size() - sz << "\n";
  std::vector<std::string> sv1;
  for(int i = (int)sz; i<all_str_vec.size(); ++i)
  {
    std::cout << all_str_vec[i - 1] << "\n";
    sv1.emplace_back(all_str_vec[i-1]);
  }
  std::cout << "xmas count: " << count_xmas_in_vec(sv1) << "\n";
  sz = all_str_vec.size();

  grd.get_all_diag_LT2RB(all_str_vec);
  std::cout << all_str_vec.size() - sz << "\n";  
  sv1.clear();
  for(int i = (int)sz; i<all_str_vec.size(); ++i)
  {
    std::cout << all_str_vec[i - 1] << "\n";
    sv1.emplace_back(all_str_vec[i-1]);
  }
  std::cout << "xmas count: " << count_xmas_in_vec(sv1) << "\n";
  sz = all_str_vec.size();

  grd.get_all_diag_RT2BL(all_str_vec);
  std::cout << all_str_vec.size() - sz << "\n";
  sv1.clear();
  for(int i = (int)sz; i<all_str_vec.size(); ++i)
  {
    std::cout << all_str_vec[i - 1] << "\n";
    sv1.emplace_back(all_str_vec[i-1]);
  }
  std::cout << "xmas count: " << count_xmas_in_vec(sv1) << "\n";
  sz = all_str_vec.size();

  return !all_str_vec.empty();
}

int main()
{
  std::vector<std::string> gRawData;

  const std::string fileP = "../puz_inp.txt";
  std::ifstream iFile(fileP);
  std::string str;
  while(iFile >> str)
  {
    gRawData.emplace_back(str);
  }
  iFile.close();

  Array2D grid2D(gRawData);

  std::cout << "Pos (0,3) :" << grid2D.get_char_at_xy(0,3) << "\n";
  std::cout << "Pos (0,3) :" << grid2D.get_char_at_xy(0,3) << "\n";
  std::cout << "Pos (2,0) :" << grid2D.get_char_at_xy(2,0) << "\n";
  std::cout << "x pos MAX :" << grid2D.get_XMAX() << "\n";
  std::cout << "y pos MAX :" << grid2D.get_YMAX() << "\n";

  std::vector<std::string> all_str_vec;
  if(get_all_arrays(grid2D,  all_str_vec))
  {
    int iTotal = count_xmas_in_vec(all_str_vec);
    std::cout << "Count of all XMAS: " << iTotal << "\n";
  }
  else
    std::cout << "\nFailed to read XMAS in file data!\n";

  return 0;
}
