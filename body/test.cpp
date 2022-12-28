/*
 * Copyright (c) 2022-2023 David Snowdon
 * Licensed under  GNU General Public License, Version 3 (the "License");
 * You may not use this file except in compliance with the License.
 * A copy of the license is contained in the file LICENSE in the root of this repo.
 */
#include <iostream>


int main()
{
  int answer = 40;
  answer += 2;
  if (answer != 42)
  {
    std::cerr << "We are all going to die!" << std::endl;
    return 2;
  }

  std::cout << "And the answer is: " << answer << std::endl;
  std::cout << "Ok." << std::endl;
  return 0;
}
