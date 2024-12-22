#include "MenuButton.h"
#include "Grid.h"

bool MenuButton::init()
{
	if(!Node::init())
		return false;
	else
		return true;
}

MenuItem* MenuButton::getMenuItem(const std::string& text, const Size& size)
{  
	const char* gb2312Text = text.c_str();
	auto label = Label::createWithSystemFont(Grid::G2U(gb2312Text), "Airal", 26);
	//label->setTextColor(Color4B(120,120,0, 255));

	auto item = MenuItemLabel::create(label);
	item->setContentSize(size);

	return item;
}
