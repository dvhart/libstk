#ifndef LIST_ITEM_H
#define LIST_ITEM_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace stk
{
	/// \todo should this extend widget ?
	/// if so we can get special event handling, drawing (which would be nice
	/// for derived classes like image_list_item, etc.).
	class list_item
	{
		public:
			typedef boost::weak_ptr<list_item> weak_ptr;
			typedef boost::shared_ptr<list_item> ptr;

		private:

		protected:
			std::wstring label_;
			std::string data_;
			list_item(std::wstring label, std::string data);

		public:
			static list_item::ptr create(std::wstring label, std::string data);
			virtual ~list_item();
			std::wstring label() const { return label_; }
			void label(const std::wstring& label) { label_ = label; }
			std::string data() const { return data_; }
			void data(const std::string& data) { data_ = data; }
	};
}

#endif
