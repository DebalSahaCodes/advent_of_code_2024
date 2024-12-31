
#include "../part5helper.hpp"

int get_pos_of_page(const PGSVEC& pgs, const PAGENUM& pg)
{
  int answer = -99;
  int pagePos = 0;
  for(const auto& s : pgs)
  {
    if(pagePos >=0 && pagePos < pgs.size() && pg == pgs[pagePos])
    {
      answer = pagePos;
      break;
    }
    ++pagePos;
  }
  return answer;
}

bool place_page_in_pos(PGSVEC& pagesVec, const PAGENUM& pg, int newPagePos)
{
  bool isPrint = false;
  if(pagesVec.empty())
  {
    if(isPrint) std::cout << "\n Empty vec!\n";
  }
  if(newPagePos >=0 && newPagePos < pagesVec.size())
  {
    const auto& pgPos = get_pos_of_page(pagesVec, pg);
    if(isPrint) std::cout << "\n Found pos:" << pgPos << "\n";
    if(pgPos < 0)
    {
      if(isPrint) std::cout << "\n pos not found for " << pg << " \n";
      return false;
    }

    const auto otherPg = pagesVec[newPagePos];
    pagesVec[newPagePos] = pg;
    pagesVec[pgPos] = otherPg;
    return true;
  }
  else
    if(isPrint) std::cout << "Bad new pos: " << newPagePos << "\n";
  return false;
}

bool isConsistent(const PGSVEC& pgs, const PGSVEC& newPgVec)
{
  bool res = pgs.size() == newPgVec.size();
  
  if(res)
  {
    for(const auto& p : newPgVec)
    {
      if(get_pos_of_page(pgs, p) < 0)
      {
        res = false;
        break;
      }
    }
    for(const auto& p : pgs)
    {
      if(get_pos_of_page(newPgVec, p) < 0)
      {
        res = false;
        break;
      }
    }
  }
  return res;
}


void get_all_pgsVec_list(const PGSVEC& pgs, std::list<PGSVEC>& pgsList)
{
  bool isPrint = false;
  const int allPos = static_cast<int>(pgs.size());
  int iPage = 0;

  for(const auto& pRef : pgs)
  {
    for(int iCount = allPos; iCount > 0; --iCount)
    {
      int pPos = iCount - 1;
      
      PGSVEC pVec = pgs;

      if(iCount == iPage)
      {
        if(isPrint) std::cout << "\n Skipping for \"" << pRef << "\" at pos " << pPos;
      }
      else
      {
        if(isPrint) std::cout << "\n Trying for \"" << pRef << "\" at pos " << pPos;

        place_page_in_pos(pVec, pRef, pPos);

        if(isConsistent(pgs, pVec))
        {
          pgsList.emplace_back(pVec);
          if(isPrint) std::cout << ".. new vector : ";
        }
        else
        {
          if(isPrint) std::cout << ".. not consistent ...\n";
        }
        if(isPrint) print_pgs_vec(pVec);
      }
    }
    ++iPage;
  }
}

bool fix_pages_vec(PGSVEC& givenPagesVec, const std::vector<RULEDATA>& rule_b)
{
  std::list<PGSVEC> pagesVecList;
  get_all_pgsVec_list(givenPagesVec, pagesVecList);
  std::cout << "\nPossible solutions to try: " << pagesVecList.size() << "\n";
  //print_pgs_vec(pages);

  for(const auto& pages : pagesVecList)
  {
    if(check_pg_numbers_vs_rules(pages, rule_b))
    {
      givenPagesVec = pages;
      return true;
    }
  }

  return !givenPagesVec.empty();
}
