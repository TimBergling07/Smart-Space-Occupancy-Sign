#pragma once
#include <cstdarg>
#include <stdint.h>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class RandomForest {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        uint8_t votes[2] = { 0 };
                        // tree #1
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.00296847359277308) {
                                if (x[3] <= 0.36685626208782196) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 0.4065352827310562) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 0.5555106699466705) {
                                        if (x[3] <= 0.4479355812072754) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 0.0040887047071009874) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= 1.5) {
                                if (x[2] <= 733.7281494140625) {
                                    if (x[2] <= 657.0500183105469) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #2
                        if (x[2] <= 326.60626220703125) {
                            if (x[0] <= 0.0033195639261975884) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 0.5233805179595947) {
                                    if (x[0] <= 0.004754379624500871) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[3] <= 0.7111665904521942) {
                                        if (x[3] <= 0.6544051170349121) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #3
                        if (x[0] <= 0.006860918831080198) {
                            if (x[3] <= 0.6921721994876862) {
                                if (x[1] <= 0.5) {
                                    if (x[0] <= 0.004718657350167632) {
                                        if (x[0] <= 0.0033195639261975884) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 0.004887501941993833) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.005867526866495609) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #4
                        if (x[2] <= 326.60626220703125) {
                            if (x[0] <= 0.003433671547099948) {
                                if (x[3] <= 0.6024583876132965) {
                                    if (x[0] <= 0.002951371599920094) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 0.0030551498057320714) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 0.4065352827310562) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 0.003910732921212912) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[0] <= 0.007579338504001498) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #5
                        if (x[1] <= 1.5) {
                            if (x[2] <= 797.9468994140625) {
                                if (x[1] <= 0.5) {
                                    if (x[0] <= 0.0025722471764311194) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 0.002595135592855513) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #6
                        if (x[2] <= 326.60626220703125) {
                            if (x[0] <= 0.0068895951844751835) {
                                if (x[0] <= 0.0025439498713240027) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[0] <= 0.0026142358547076583) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 0.32224762439727783) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[2] <= 664.3531494140625) {
                                if (x[0] <= 0.0066518839448690414) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #7
                        if (x[0] <= 0.00634397380053997) {
                            if (x[0] <= 0.0025439498713240027) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 0.0026142358547076583) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 0.4065352827310562) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 0.7111665904521942) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.006889137206599116) {
                                if (x[2] <= 738.1968994140625) {
                                    if (x[2] <= 657.0500183105469) {
                                        if (x[3] <= 0.630575954914093) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #8
                        if (x[0] <= 0.0069272564724087715) {
                            if (x[3] <= 0.7111665904521942) {
                                if (x[0] <= 0.005171544151380658) {
                                    if (x[0] <= 0.0025722471764311194) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 0.6391853392124176) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 0.6305623054504395) {
                                        if (x[2] <= 657.0500183105469) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 0.00647886679507792) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 0.7148007452487946) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #9
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.002951371599920094) {
                                if (x[0] <= 0.0025722471764311194) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[0] <= 0.0025933391880244017) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 0.4065352827310562) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 0.7111665904521942) {
                                        if (x[0] <= 0.003910732921212912) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #10
                        if (x[1] <= 0.5) {
                            if (x[3] <= 0.4358087033033371) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[3] <= 0.6297551989555359) {
                                    if (x[0] <= 0.0053507015109062195) {
                                        if (x[3] <= 0.5371022820472717) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 0.006013485603034496) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 0.7034127414226532) {
                                        if (x[3] <= 0.6523759663105011) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 0.7073883712291718) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[2] <= 664.3531494140625) {
                                if (x[2] <= 657.0500183105469) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #11
                        if (x[2] <= 326.60626220703125) {
                            if (x[0] <= 0.00296847359277308) {
                                if (x[0] <= 0.0025722471764311194) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 0.36685626208782196) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 0.5427409410476685) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 0.003030567313544452) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 0.6921721994876862) {
                                        if (x[3] <= 0.4065352827310562) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 0.00582566624507308) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #12
                        if (x[1] <= 0.5) {
                            if (x[3] <= 0.4358087033033371) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[0] <= 0.007579338504001498) {
                                    if (x[3] <= 0.6921721994876862) {
                                        if (x[0] <= 0.0033195639261975884) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 0.6987465918064117) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 664.3531494140625) {
                                if (x[0] <= 0.0066518839448690414) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #13
                        if (x[2] <= 327.9125061035156) {
                            if (x[0] <= 0.0033195639261975884) {
                                if (x[3] <= 0.36685626208782196) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 0.0025722471764311194) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 0.5341479480266571) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 0.004103074315935373) {
                                    if (x[0] <= 0.0034623051760718226) {
                                        if (x[3] <= 0.5766396224498749) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 0.5704882740974426) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[0] <= 0.007579338504001498) {
                                        if (x[3] <= 0.42000700533390045) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #14
                        if (x[0] <= 0.0069272564724087715) {
                            if (x[0] <= 0.0025722471764311194) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 738.1968994140625) {
                                    if (x[0] <= 0.0025947631802409887) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[0] <= 0.0034623051760718226) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #15
                        if (x[2] <= 664.3531494140625) {
                            if (x[0] <= 0.0039816314820200205) {
                                if (x[3] <= 0.3964139521121979) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 0.002539680222980678) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 0.5341479480266571) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 657.0500183105469) {
                                    if (x[2] <= 327.9125061035156) {
                                        if (x[0] <= 0.004103074315935373) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // return argmax of votes
                        uint8_t classIdx = 0;
                        float maxVotes = votes[0];

                        for (uint8_t i = 1; i < 2; i++) {
                            if (votes[i] > maxVotes) {
                                classIdx = i;
                                maxVotes = votes[i];
                            }
                        }

                        return classIdx;
                    }

                    /**
                    * Predict readable class name
                    */
                    const char* predictLabel(float *x) {
                        return idxToLabel(predict(x));
                    }

                    /**
                    * Convert class idx to readable name
                    */
                    const char* idxToLabel(uint8_t classIdx) {
                        switch (classIdx) {
                            case 0:
                            return "Empty";
                            case 1:
                            return "Occupied";
                            default:
                            return "Houston we have a problem";
                        }
                    }

                protected:
                };
            }
        }
    }