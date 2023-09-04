//
// Created by prlpr on 03/09/2023.
//

#include "PrimUtilities.hpp"

std::string QuadrantStateEnum2String(QuadrantStateEnum state) {
    switch (state) {
        case QuadrantStateEnum::DO:
            return "DO";
        case QuadrantStateEnum::SCHEDULE:
            return "SCHEDULE";
        case QuadrantStateEnum::DELEGATE:
            return "DELEGATE";
        case QuadrantStateEnum::DELETE:
            return "DELETE";
        default:
            return "UNKNOWN";
    }
}
