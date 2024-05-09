// A program that implements the FIFO, OPT, LRU, and CLOCK
// page replacement algorithms presented in Chapter 9.

#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

// Reference string generation using c++ random library
std::vector<int> generateReferenceString(const int size);

// Different Algorithm implementation
void FIFOImpl(const std::vector<int> &referenceString, const int frameSize);
void OPTImpl(const std::vector<int> &referenceString, const int frameSize);
void LRUImpl(const std::vector<int> &referenceString, const int frameSize);
void CLOCKImpl(const std::vector<int> &referenceString, const int frameSize);

// Show the info of the reference string
void showInfo(const std::vector<int> &referenceString);

int main(int argc, char const *argv[]) {
  // size of the reference string
  constexpr int REF_STR_SIZE = 20;
  // number of the frames available
  constexpr int FRAME_SIZE = 3;

  // generate a random page-reference string where page numbers range is [0, 9]
  const auto referenceString = generateReferenceString(REF_STR_SIZE);

  // Show the info of the reference string
  showInfo(referenceString);
  std::cout << "\nFIFO page replacement algorithm: \n";
  FIFOImpl(referenceString, FRAME_SIZE);
  std::cout << "\nOPT page replacement algorithm: \n";
  OPTImpl(referenceString, FRAME_SIZE);
  std::cout << "\nLRU page replacement algorithm: \n";
  LRUImpl(referenceString, FRAME_SIZE);
  std::cout << "\nCLOCK page replacement algorithm: \n";
  CLOCKImpl(referenceString, FRAME_SIZE);

  return 0;
}

// Reference string generation using c++ random library
std::vector<int> generateReferenceString(const int size) {
  std::random_device dev;
  std::mt19937 mt(dev());
  // Distribution from 0 to 9,([0, 9])
  std::uniform_int_distribution<int> dist(0, 9);

  std::vector<int> result(size);
  for (auto &num : result) {
    num = dist(mt);
  }
  return result;
}

// Different Algorithm implementation

void FIFOImpl(const std::vector<int> &referenceString, const int frameSize) {
  // Frame vector
  // -1 means no page reference in frame
  std::vector<int> frame(frameSize, -1);
  // History to tell whether the memory reference causes page fault
  std::vector<bool> pageFaultHistory(referenceString.size(), false);

  // This function finds the first page refernce into the frame
  // If found, return the index of the page frame
  // Else, return -1
  auto prevSearch = [&](const int refIndex) -> int {
    // 1. Find if the page frame contains -1
    for (int i = 0; i < frame.size(); i++) {
      // If there is a -1(null), return directly
      if (frame[i] == -1) {
        return i;
      }
    }

    // 2. foreach page reference in the frame, find the index in the refString
    // where the refernce enters the frame, then calculate the minimum number of
    // the index and record the corresponding index, finally, return the
    // corresponding index
    int earliestEntryIndex = referenceString.size();
    int earliestEntryIndexInFrame = -1;

    for (int i = 0; i < frame.size(); i++) {
      for (int j = refIndex - 1; j >= 0; j--) {
        // If the page number dont match, not found
        if (frame[i] != referenceString[j]) {
          continue;
        }
        // No page fault means the reference does not enter the frame
        if (pageFaultHistory[j] == false) {
          continue;
        }
        // Match, calculate the minimum value, then break, no search anymore
        if (j < earliestEntryIndex) {
          earliestEntryIndex = j;
          earliestEntryIndexInFrame = i;
        }
        break;
      }
    }

    return earliestEntryIndexInFrame;
  };

  auto logging = [&](const bool pageFaultFlag = false, const int refIndex = -1,
                     const int replacedFrameNumber = -1) -> void {
    // at the beginning
    if (refIndex == -1) {
      std::cout << "No page is in the page frame, the page frame is: [";
      for (const auto pageNumber : frame) {
        std::cout << pageNumber << ", ";
      }
      std::cout << "\b\b]\n";
      return;
    }

    std::cout << "Page " << referenceString[refIndex] << " reference, ";
    // If no page fault
    if (pageFaultFlag == false) {
      std::cout << "no page fault.\n";
      return;
    }

    // Page fault
    if (replacedFrameNumber == -1) {
      std::cout << "bring to the page frame: [";
      for (const auto pageNumber : frame) {
        std::cout << pageNumber << ", ";
      }
      std::cout << "\b\b]\n";
      return;
    }
    std::cout << "page fault, " << replacedFrameNumber
              << " is the first in, replace " << replacedFrameNumber << ": [";
    for (const auto pageNumber : frame) {
      std::cout << pageNumber << ", ";
    }
    std::cout << "\b\b]\n";
  };

  // Show default info
  logging();

  for (int i = 0; i < referenceString.size(); i++) {
    // First, search the page frame to find whether there is a page fault
    bool pageFaultFlag = true;
    for (int j = 0; j < frame.size(); j++) {
      if (referenceString[i] != frame[j]) {
        continue;
      }
      // if a match is found, no page fault, break
      pageFaultFlag = false;
      break;
    }

    pageFaultHistory[i] = pageFaultFlag;
    // std::cout << "pageFaultFlag: " << pageFaultFlag << "\n";

    // No page fault means no need to find a index in prev, continue
    if (pageFaultFlag == false) {
      logging(pageFaultFlag, i);
      continue;
    }

    // Otherwise there is a page fault
    // Find the page to replace
    const int toReplaceIndexInFrame = prevSearch(i);
    // std::cout << "toReplaceIndexInFrame: " << toReplaceIndexInFrame << "\n";

    // Not found, print error
    if (toReplaceIndexInFrame == -1) {
      std::cerr << "Error: toReplaceIndexInFrame not found in FIFOImpl\n";
      exit(1);
    }

    const int replcedPageNumber = frame[toReplaceIndexInFrame];

    // Found the index, replace
    frame[toReplaceIndexInFrame] = referenceString[i];

    logging(pageFaultFlag, i, replcedPageNumber);
  }
}

void OPTImpl(const std::vector<int> &referenceString, const int frameSize) {
  // Frame vector
  // -1 means no page reference in frame
  std::vector<int> frame(frameSize, -1);

  // This function finds the last page refernce into the frame
  // If found, return the index of the page frame
  // Else, return -1
  auto nextSearch = [&](const int refIndex) -> int {
    // 1. Find if the page frame contains -1
    for (int i = 0; i < frame.size(); i++) {
      // If there is a -1(null), return directly
      if (frame[i] == -1) {
        return i;
      }
    }

    // 2. foreach page reference in the frame, find the index in the refString
    // where the refernce last occurs(used the lateset in the future), then
    // calculate the maximum number of the index and record the corresponding
    // index, finally, return the corresponding index
    int latestUsedIndex = -1;
    int latestUsedIndexInFrame = -1;

    for (int i = 0; i < frame.size(); i++) {
      bool foundFlag = false;
      for (int j = refIndex + 1; j < referenceString.size(); j++) {
        // If the page number dont match, not found
        if (frame[i] != referenceString[j]) {
          continue;
        }
        // Match, calculate the minimum value, then break, no search anymore
        foundFlag = true;
        if (j > latestUsedIndex) {
          latestUsedIndex = j;
          latestUsedIndexInFrame = i;
        }
        break;
      }
      // If current page number not found in the next reference string,
      // this page will not be used anymore
      // return this page directly
      if (foundFlag == false) {
        return i;
      }
    }

    return latestUsedIndexInFrame;
  };

  auto logging = [&](const bool pageFaultFlag = false, const int refIndex = -1,
                     const int replacedFrameNumber = -1) -> void {
    // at the beginning
    if (refIndex == -1) {
      std::cout << "No page is in the page frame, the page frame is: [";
      for (const auto pageNumber : frame) {
        std::cout << pageNumber << ", ";
      }
      std::cout << "\b\b]\n";
      return;
    }

    std::cout << "Page " << referenceString[refIndex] << " reference, ";
    // If no page fault
    if (pageFaultFlag == false) {
      std::cout << "no page fault.\n";

      return;
    }

    // Page fault
    if (replacedFrameNumber == -1) {
      std::cout << "bring to the page frame: [";
      for (const auto pageNumber : frame) {
        std::cout << pageNumber << ", ";
      }
      std::cout << "\b\b]\n";
      return;
    }
    std::cout << "page fault, " << replacedFrameNumber
              << " will not be used for the longest, replace "
              << replacedFrameNumber << ": [";
    for (const auto pageNumber : frame) {
      std::cout << pageNumber << ", ";
    }
    std::cout << "\b\b]\n";
  };

  // Show default info
  logging();

  for (int i = 0; i < referenceString.size(); i++) {
    // First, search the page frame to find whether there is a page fault
    bool pageFaultFlag = true;
    for (int j = 0; j < frame.size(); j++) {
      if (referenceString[i] != frame[j]) {
        continue;
      }
      // if a match is found, no page fault, break
      pageFaultFlag = false;
      break;
    }

    // std::cout << "pageFaultFlag: " << pageFaultFlag << "\n";

    // No page fault means no need to find a index in prev, continue
    if (pageFaultFlag == false) {
      logging(pageFaultFlag, i);
      continue;
    }

    // Otherwise there is a page fault
    // Find the page to replace
    const int toReplaceIndexInFrame = nextSearch(i);
    // std::cout << "toReplaceIndexInFrame: " << toReplaceIndexInFrame << "\n";

    // Not found, print error
    if (toReplaceIndexInFrame == -1) {
      std::cerr << "Error: toReplaceIndexInFrame not found in OPTImpl\n";
      exit(1);
    }

    const int replcedPageNumber = frame[toReplaceIndexInFrame];

    // Found the index, replace
    frame[toReplaceIndexInFrame] = referenceString[i];
    logging(pageFaultFlag, i, replcedPageNumber);
  }
}

void LRUImpl(const std::vector<int> &referenceString, const int frameSize) {
  // Frame vector
  // -1 means no page reference in frame
  std::vector<int> frame(frameSize, -1);

  // This function finds the earliest access page number
  // If found, return the index of the page frame
  // Else, return -1
  auto prevSearch = [&](const int refIndex) -> int {
    // 1. Find if the page frame contains -1
    for (int i = 0; i < frame.size(); i++) {
      // If there is a -1(null), return directly
      if (frame[i] == -1) {
        return i;
      }
    }

    // 2. foreach page reference in the frame, find the index in the refString
    // where the refernce earliest occurs (least recently used), then calculate
    // the minimum number of the index and record the corresponding index,
    // finally, return the corresponding index
    int earliestUsedIndex = referenceString.size();
    int earliestUsedIndexInFrame = -1;

    for (int i = 0; i < frame.size(); i++) {
      for (int j = refIndex - 1; j >= 0; j--) {
        // If the page number dont match, not found
        if (frame[i] != referenceString[j]) {
          continue;
        }
        // Match, calculate the minimum value, then break, no search anymore
        if (j < earliestUsedIndex) {
          earliestUsedIndex = j;
          earliestUsedIndexInFrame = i;
        }
        break;
      }
    }

    return earliestUsedIndexInFrame;
  };

  auto logging = [&](const bool pageFaultFlag = false, const int refIndex = -1,
                     const int replacedFrameNumber = -1) -> void {
    // at the beginning
    if (refIndex == -1) {
      std::cout << "No page is in the page frame, the page frame is: [";
      for (const auto pageNumber : frame) {
        std::cout << pageNumber << ", ";
      }
      std::cout << "\b\b]\n";
      return;
    }

    std::cout << "Page " << referenceString[refIndex] << " reference, ";
    // If no page fault
    if (pageFaultFlag == false) {
      std::cout << "no page fault.\n";

      return;
    }

    // Page fault
    if (replacedFrameNumber == -1) {
      std::cout << "bring to the page frame: [";
      for (const auto pageNumber : frame) {
        std::cout << pageNumber << ", ";
      }
      std::cout << "\b\b]\n";
      return;
    }
    std::cout << "page fault, " << replacedFrameNumber
              << " is least used, replace " << replacedFrameNumber << ": [";
    for (const auto pageNumber : frame) {
      std::cout << pageNumber << ", ";
    }
    std::cout << "\b\b]\n";
  };

  // Show default info
  logging();

  for (int i = 0; i < referenceString.size(); i++) {
    // First, search the page frame to find whether there is a page fault
    bool pageFaultFlag = true;
    for (int j = 0; j < frame.size(); j++) {
      if (referenceString[i] != frame[j]) {
        continue;
      }
      // if a match is found, no page fault, break
      pageFaultFlag = false;
      break;
    }

    // std::cout << "pageFaultFlag: " << pageFaultFlag << "\n";

    // No page fault means no need to find a index in prev, continue
    if (pageFaultFlag == false) {
      logging(pageFaultFlag, i);
      continue;
    }

    // Otherwise there is a page fault
    // Find the page to replace
    const int toReplaceIndexInFrame = prevSearch(i);
    // std::cout << "toReplaceIndexInFrame: " << toReplaceIndexInFrame << "\n";

    // Not found, print error
    if (toReplaceIndexInFrame == -1) {
      std::cerr << "Error: toReplaceIndexInFrame not found in LRUImpl\n";
      exit(1);
    }

    const int replcedPageNumber = frame[toReplaceIndexInFrame];

    // Found the index, replace
    frame[toReplaceIndexInFrame] = referenceString[i];
    logging(pageFaultFlag, i, replcedPageNumber);
  }
}

void CLOCKImpl(const std::vector<int> &referenceString, const int frameSize) {
  // Basic frame structure
  struct Frame {
    int pageNumber;
    bool referenceBit;
    Frame(const int pageNumber = -1, const bool referenceBit = false)
        : pageNumber(pageNumber), referenceBit(referenceBit) {}
  };

  // Frame vector
  std::vector<Frame> frame(frameSize);
  // initalize clock
  int clock = 0;

  // This function acts as a clock.
  // A pointer (that is, a hand on the clock) indicates which page is to be
  // replaced next. When a frame is needed, the pointer advances until it ﬁnds a
  // page with a 0 reference bit. As it advances, it clears the reference bits.
  // Once a victim page is found, the page is replaced, and the new page is
  // inserted in the circular queue in that position.
  // This fuction returns a index to the frame to be replaced
  auto circularSearch = [&]() -> int {
    while (frame[clock].referenceBit == true) {
      // set reference bit to 0
      frame[clock].referenceBit = false;
      // advance 1 position
      clock = (clock + 1) % frameSize;
    }
    // found a frame that refernce bit is 0
    // which means it can be replaced
    return clock;
  };

  auto logging = [&](const bool pageFaultFlag = false, const int refIndex = -1,
                     const int replacedFrameNumber = -1) -> void {
    // at the beginning
    if (refIndex == -1) {
      std::cout << "No page is in the page frame, the page frame is: [";
      for (const auto fr : frame) {
        std::cout << fr.pageNumber << ", ";
      }
      std::cout << "\b\b]\n";
      return;
    }

    std::cout << "Page " << referenceString[refIndex] << " reference, ";
    // If no page fault
    if (pageFaultFlag == false) {
      std::cout << "no page fault.\n";
      return;
    }

    // Page fault
    if (replacedFrameNumber == -1) {
      std::cout << "bring to the page frame: [";
      for (const auto fr : frame) {
        std::cout << fr.pageNumber << ", ";
      }
      std::cout << "\b\b]\n";
      return;
    }
    std::cout << "page fault, " << replacedFrameNumber
              << "'s reference bit is 0, replace " << replacedFrameNumber
              << ": [";
    for (const auto fr : frame) {
      std::cout << fr.pageNumber << ", ";
    }
    std::cout << "\b\b]\n";
  };

  // Show default info
  logging();

  for (int i = 0; i < referenceString.size(); i++) {
    // First, search the page frame to find whether there is a page fault
    bool pageFaultFlag = true;
    for (int j = 0; j < frame.size(); j++) {
      if (referenceString[i] != frame[j].pageNumber) {
        continue;
      }
      // if a match is found, no page fault, break
      pageFaultFlag = false;
      // also, set the reference bit to 1
      frame[j].referenceBit = true;
      break;
    }

    // No page fault means no need to find a index in prev, continue
    if (pageFaultFlag == false) {
      logging(pageFaultFlag, i);
      continue;
    }

    // Otherwise there is a page fault
    // Find the page to replace
    const int toReplaceIndexInFrame = circularSearch();
    // std::cout << "toReplaceIndexInFrame: " << toReplaceIndexInFrame << "\n";

    // Not found, print error
    if (toReplaceIndexInFrame == -1) {
      std::cerr << "Error: toReplaceIndexInFrame not found in CLOCKImpl\n";
      exit(1);
    }

    const int replcedPageNumber = frame[toReplaceIndexInFrame].pageNumber;

    // Found the index, replace
    frame[toReplaceIndexInFrame].pageNumber = referenceString[i];
    // then set the reference bit to 1
    frame[toReplaceIndexInFrame].referenceBit = true;
    logging(pageFaultFlag, i, replcedPageNumber);
  }
}

// Show the info of the reference string
void showInfo(const std::vector<int> &referenceString) {
  std::cout << "The reference string is: [";
  for (const auto num : referenceString) {
    std::cout << num << ", ";
  }
  std::cout << "\b\b]\n";
}
