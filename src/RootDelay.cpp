// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
#include "RootDelay.h"

namespace CPlantBox {

/**
 * Deep copies the organ into the new plant @param rs.
 * All laterals are deep copied, plant and parent pointers are updated.
 *
 * @param plant     the plant the copied organ will be part of
 */
std::shared_ptr<Organ> RootDelay::copy(std::shared_ptr<Organism> rs)
{
    auto r = std::make_shared<RootDelay>(*this); // shallow copy
    r->parent = std::weak_ptr<Organ>();
    r->plant = rs;
    r->param_ = std::make_shared<RootSpecificParameter>(*param()); // copy parameters
    for (size_t i=0; i< children.size(); i++) {
        r->children[i] = children[i]->copy(rs); // copy laterals
        r->children[i]->setParent(r);
    }
    return r;
}

/**
 * @return Quick info about the object for debugging
 * additionally, use getParam()->toString() and getOrganRandomParameter()->toString() to obtain all information.
 */
std::string RootDelay::toString() const
{
    std::string str = Organ::toString();
    str.replace(0, 6, "Delay");
    std::stringstream newstring;
    newstring << "; initial heading: " << iHeading.toString() << ", parent base length " << parentBaseLength << ", parent node index" << parentNI << ".";
    return str+newstring.str();
}

/**
 * Creates a new lateral root and passes time overhead
 *
 * Overwrite this method to implement more spezialized root classes.
 *
 * @param verbose   turns console output on or off
 */
void RootDelay::createLateral(bool verbose)
{
	// std::cout<< "create delayed root\n";
	auto rrp = getRootRandomParameter(); // rename
    int lt = rrp->getLateralType(nodes.back());
    if (lt>0) {
    	double delay = std::max(rrp->ldelay + plant.lock()->randn()*rrp->ldelays, 0.);
        auto lateral = std::make_shared<RootDelay>(plant.lock(), lt,  heading(), delay,  shared_from_this(), length, nodes.size()-1);
        children.push_back(lateral);
    	double ageLN = this->calcAge(length); // age of root when lateral node is created
        lateral->simulate(age-ageLN,verbose); // pass time overhead (age we want to achieve minus current age)
    }
}

} // end namespace CPlantBox
