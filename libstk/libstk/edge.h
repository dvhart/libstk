#ifndef STK_EDGE_H
#define STK_EDGE_H

namespace stk
{
	// edge used for scanfill of polygons
	class edge
	{
		private:
			int ymax_, xbot_, xinc_num_, xinc_den_, xinc_, xinc_dir_;
		public:
			edge(int ymax, int xbot, int xinc_num, int xinc_den) :
				ymax_(ymax), xbot_(xbot), xinc_num_(xinc_num),
			xinc_den_(xinc_den), xinc_(0), xinc_dir_(((xinc_num>0)?1:-1))
			{};
			int ymax() const { return ymax_; };
			int xbot() const { return xbot_; };
			int xinc_num() const { return xinc_num_; };
			int xinc_den() const { return xinc_den_; };
			int xinc() const { return xinc_; };
			void ymax(int ymax) { ymax_ = ymax; };
			void xbot(int xbot)	{	xbot_ = xbot;	};
			void xinc_num(int xinc_num)	{ xinc_num_ = xinc_num; };
			void xinc_den(int xinc_den)	{	xinc_den_ = xinc_den;	};
			void xinc(int xinc)	{ xinc_ = xinc;	};

			void step()
			{
				xinc_ += xinc_num_;
				while (abs(xinc_) >= xinc_den_)
				{
					xbot_ += xinc_dir_;
					if (xinc_dir_ > 0)
						xinc_ -= xinc_den_;
					else
						xinc_ += xinc_den_;
				}
			};

			bool operator<(const edge &ed) const
			{
				if (xbot_ < ed.xbot())
					return true;
				else
					return false;
			};
	};
}


#endif
