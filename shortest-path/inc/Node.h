#ifndef NODE_H
#define NODE_H

class Node {
public:

	Node();
	Node(char name, float huristic);
	virtual ~Node();

	Node& operator=(const Node& right);

	/**
	 * @returns {bool} true if the calling node name is the same as the goalCity, and returns false otherwise.
	 */
	bool isGoal(char goalCity);
	bool isExplored() const;

	char getName() const;
	Node* getParent() const;
	Node* getNext() const;
	float getCost() const;
	float getHuristic() const;
	int getExpanded() const;

	float calcCost(int algo);

	void expand(std::vector<Link> &links, std::map<char, float>& huristicMap, List* pFrontier);
	void pushNodeToList(List* pFrontier);
	void rmNodeFromList(List* pFrontier);
	bool isQualified(List* pFrontier);

	friend std::ostream& operator<<(std::ostream& os, const Node& obj);

private:
	char name;
	Node* pParent;
	Node* pNext;
	float cost;
	float huristic;
	bool explored;
	int expanded;
};

#endif /* NODE_H */