#include "grower.h"
#include "mathfunc.h"
#include "errorhandler.h"

extern ErrorHandler handle;

//Uses the length increase in interpLengthGrowth and mean weight change in
//interpWeightGrowth to calculate lgrowth and wgrowth.
void Grower::implementGrowth(int area, const PopInfoVector& NumberInArea,
  const LengthGroupDivision* const Lengths) {

  int lgroup, j, inarea = this->areaNum(area);
  double meanw, dw, tmppart3, tmpweight;
  double tmpMult = growthcalc->getMult();
  double tmpPower = growthcalc->getPower();
  double tmpDl = 1.0 / Lengths->dl();  //JMB no need to check zero here

  for (lgroup = 0; lgroup < Lengths->numLengthGroups(); lgroup++) {
    meanw = 0.0;
    part3 = 1.0;
    tmpweight = (NumberInArea[lgroup].W * tmpPower * Lengths->dl()) / Lengths->meanLength(lgroup);
    growth = interpLengthGrowth[inarea][lgroup] * tmpDl;
    if (growth >= maxlengthgroupgrowth)
      growth = double(maxlengthgroupgrowth) - 0.1;
    if (growth < verysmall)
      growth = 0.0;
    alpha = beta * growth / (maxlengthgroupgrowth - growth);
    for (j = 0; j < maxlengthgroupgrowth; j++)
      part3 *= (alpha + beta + double(j));

    tmppart3 = 1.0 / part3;
    part4[1] = alpha;
    if (maxlengthgroupgrowth > 1)
      for (j = 2; j <= maxlengthgroupgrowth; j++)
        part4[j] = part4[j - 1] * (j - 1 + alpha);

    if (functionnumber == 8) {
      for (j = 0; j <= maxlengthgroupgrowth; j++) {
        (*lgrowth[inarea])[j][lgroup] = part1[j] * part2[j] * tmppart3 * part4[j];
        (*wgrowth[inarea])[j][lgroup] = tmpMult * (pow(Lengths->meanLength(lgroup + j), tmpPower) - pow(Lengths->meanLength(lgroup), tmpPower));
      }

    } else {
      for (j = 0; j <= maxlengthgroupgrowth; j++) {
        (*lgrowth[inarea])[j][lgroup] = part1[j] * part2[j] * tmppart3 * part4[j];
        (*wgrowth[inarea])[j][lgroup] = tmpweight * j;
        meanw += (*wgrowth[inarea])[j][lgroup] * (*lgrowth[inarea])[j][lgroup];
      }

      dw = interpWeightGrowth[inarea][lgroup] - meanw;
      for (j = 0; j <= maxlengthgroupgrowth; j++)
        (*wgrowth[inarea])[j][lgroup] += dw;
    }
  }
}

//Uses only the length increase in interpLengthGrowth to calculate lgrowth.
void Grower::implementGrowth(int area, const LengthGroupDivision* const Lengths) {

  int lgroup, j, inarea = this->areaNum(area);
  double tmppart3;
  double tmpDl = 1.0 / Lengths->dl();  //JMB no need to check zero here

  for (lgroup = 0; lgroup < Lengths->numLengthGroups(); lgroup++) {
    part3 = 1.0;
    growth = interpLengthGrowth[inarea][lgroup] * tmpDl;
    if (growth >= maxlengthgroupgrowth)
      growth = double(maxlengthgroupgrowth) - 0.1;
    if (growth < verysmall)
      growth = 0.0;
    alpha = beta * growth / (maxlengthgroupgrowth - growth);
    for (j = 0; j < maxlengthgroupgrowth; j++)
      part3 *= (alpha + beta + double(j));

    tmppart3 = 1.0 / part3;
    part4[1] = alpha;
    if (maxlengthgroupgrowth > 1)
      for (j = 2; j <= maxlengthgroupgrowth; j++)
        part4[j] = part4[j - 1] * (j - 1 + alpha);

    for (j = 0; j <= maxlengthgroupgrowth; j++)
      (*lgrowth[inarea])[j][lgroup] = part1[j] * part2[j] * tmppart3 * part4[j];
  }
}
