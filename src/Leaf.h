#ifndef LEAF_H_
#define LEAF_H_

#include "Organ.h"
#include "Organism.h"
#include "leafparameter.h"

#include <iostream>
#include <assert.h>

namespace CPlantBox {

/**
 * Stem
 *
 * Describes a single stem, by a vector of nodes representing the stem.
 * The method simulate() creates new nodes of this stem, and lateral stems in the stem's branching zone.
 *
 */
class Leaf : public Organ
{
public:

    static std::vector<int> leafphytomerID;

    Leaf(int id,  std::shared_ptr<const OrganSpecificParameter> param, bool alive, bool active, double age, double length,
        Vector3d iheading, double pbl, int pni, bool moved = false, int oldNON = 0);
	Leaf(std::shared_ptr<Organism> plant, int type, Vector3d iheading, double delay, std::shared_ptr<Organ> parent, double pbl, int pni); ///< used within simulation
	virtual ~Leaf() { };

	std::shared_ptr<Organ> copy(std::shared_ptr<Organism> plant) override;   ///< deep copies the root tree

	int organType() const override { return Organism::ot_leaf; } ///< returns the organs type

	void simulate(double dt, bool silence = false) override; ///< stem growth for a time span of \param dt

	double getParameter(std::string name) const override; ///< returns an organ parameter of Plant::ScalarType

    std::string toString() const override;

	/* exact from analytical equations */
	double calcCreationTime(double lenght); ///< analytical creation (=emergence) time of a node at a length
	double calcLength(double age); ///< analytical length of the stem
	double calcAge(double length); ///< analytical age of the stem

	/* abbreviations */
	std::shared_ptr<LeafRandomParameter> getLeafRandomParameter() const;  ///< root type parameter of this root
	std::shared_ptr<const LeafSpecificParameter> param() const; ///< root parameter
	double dx() const; ///< returns the axial resolution

	Vector3d iHeading; ///< the initial heading of the leaf, when it was created
    double parentBaseLength; ///< length [cm]
    int parentNI; ///< parent node index

protected:

    void minusPhytomerId(int subtype) { leafphytomerID[subtype]--;  }
    int getleafphytomerID(int subtype) { return leafphytomerID[subtype]; }
    void addleafphytomerID(int subtype) { leafphytomerID[subtype]++;  }

    void createLateral(bool silence); ///< creates a new lateral, called by Leaf::simulate()

    Vector3d heading() const; /// current heading of the root tip
    virtual Vector3d getIncrement(const Vector3d& p, double sdx); ///< called by createSegments, to determine growth direction
	void createSegments(double l, bool silence); ///< creates segments of length l, called by stem::simulate()

    bool firstCall = true;
    const double smallDx = 1e-6; ///< threshold value, smaller segments will be skipped (otherwise stem tip direction can become NaN)
};

} // namespace CPlantBox

#endif
