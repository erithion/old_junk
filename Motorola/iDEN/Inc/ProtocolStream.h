#ifndef _PROTOCOL_STREAM_H_
#define _PROTOCOL_STREAM_H_

namespace protocol
{
	template <typename PortInputAdapter>
	class policyDirect
	{
	public:
		streamInput(PortInputAdapter& port) : port_(port)
		{
		}
	protected:
	private:
		PortInputAdapter port_;
	};
}
#endif // _PROTOCOL_STREAM_H_