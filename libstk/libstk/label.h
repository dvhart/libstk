#ifndef STK_LABEL_H
#define STK_LABEL_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include "widget.h"
#include "container.h"

namespace stk
{
	class label : public widget
	{
		public:
			typedef boost::shared_ptr<label> ptr;
			typedef boost::weak_ptr<label> weak_ptr;

		private:
			std::wstring text_;
			
		protected:
			label(container::ptr parent, std::wstring text, const rectangle& rect);
			
		public:
			static label::ptr create(container::ptr parent, std::wstring text, 
					const rectangle& rect);
			virtual ~label();

			std::wstring text() const { return text_; }
			void text(const std::wstring& t) { text_ = t; }

			virtual void draw(boost::shared_ptr<stk::surface> surface);
	};
	
} // namespace stk

#endif
