#include <string>
#include <boost/shared_ptr.hpp>

class XNode
{
public:
	XNode();
	~XNode();

private:

};

XNode::XNode()
{
}

XNode::~XNode()
{
}
typedef boost::shared_ptr<XNode> PNode;