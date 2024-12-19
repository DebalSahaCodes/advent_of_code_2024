#include<iostream>
#include<fstream>
#include<list>
#include<vector>
#include<string>

using POS_TYPE = std::pair<int, int>; // for coordinate

bool check_for_mas_pos(const std::string& str)
{
  return str.size() >= 3 && 
  ("MAS" == str.substr(str.size() - 3,3) || "SAM" == str.substr(str.size() - 3,3));
}

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

  void get_all_diag_LT2RB(std::list<POS_TYPE>& masPos) const // Left-TOP to Right-Bottom
  {
    int old_x, old_y = 0;

    const int XMX = get_XMAX();
    int offset = 0; //for upper right half
    while(offset <= XMX)
    {
      std::string str;
      for(int x=offset, y=0; x<=XMX && y<=get_YMAX(); ++x,++y)
      {
        str += get_char_at_xy(x,y);
        if(check_for_mas_pos(str))
        {
          masPos.emplace_back(POS_TYPE({old_x, old_y}));
        }
        old_x = x;
        old_y = y;
      }
      ++offset;
    }
    offset = 1; //for lower left half (0 already done)
    while(offset <= XMX)
    {
      std::string str;
      for(int x=0, y=offset; x<=XMX && y<=get_YMAX(); ++x,++y)
      {
        str += get_char_at_xy(x,y);
        if(check_for_mas_pos(str))
        {
          masPos.emplace_back(POS_TYPE({old_x, old_y}));
        }
        old_x = x;
        old_y = y;
      }
      //std::cout << "Diag : " << str << "\n";
      ++offset;
    }
  }

  void get_all_diag_RT2BL(std::list<POS_TYPE>& masPos) const // right-TOP to Bottom-left
  {
    int old_x, old_y;
    const int XMX = get_XMAX();
    int offset = XMX; //lower right
    while(offset>=0)
    {
      std::string str;
      for(int x=offset, y=0; x>=0 && y<=get_YMAX(); --x,++y)
      {
        str += get_char_at_xy(x,y);
        if(check_for_mas_pos(str))
        {
          masPos.emplace_back(POS_TYPE({old_x, old_y}));
        }
        old_x = x;
        old_y = y;
      }
      --offset;
    }
    offset = 1; //upper left (0 already done)
    while(offset <= XMX)
    {
      std::string str;
      for(int x=XMX, y=offset; x>=0 && y<=get_YMAX(); --x,++y)
      {
        str += get_char_at_xy(x,y);
        if(check_for_mas_pos(str))
        {
          masPos.emplace_back(POS_TYPE({old_x, old_y}));
        }
        old_x = x;
        old_y = y;
      }
      ++offset;
    }
  }

};


int count_all_mas(const Array2D& grd)
{
  std::list<POS_TYPE> all_A_pos1;
  std::list<POS_TYPE> all_A_pos2;

  grd.get_all_diag_LT2RB(all_A_pos1);
  grd.get_all_diag_RT2BL(all_A_pos2);

  std::cout << "\nSize of all_A_pos1 : " << all_A_pos1.size() << "\n";
  std::cout << "\nSize of all_A_pos2 : " << all_A_pos2.size() << "\n";

  int t = 0;
  for(const auto& s1 : all_A_pos1)
  {
    for (const auto& s2 : all_A_pos2)
    {
      if(s1 == s2)
        ++t;
    }
  }
  
  return t;
}

int main()
{
  std::vector<std::string> gRawData;

  //const std::string fileP = "sam_inp_p2.txt";
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

  int iTotal = count_all_mas(grid2D);
  if(iTotal > 0)
  {
    std::cout << "Count of all MAS: " << iTotal << "\n";
  }
  else
    std::cout << "\nFailed to read MAS in file data!\n";

  return 0;
}
