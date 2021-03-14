#include "Common.h"

using namespace std;

class Rectangle : public IShape {
public:
	unique_ptr<IShape> Clone() const override
	{
		unique_ptr<IShape> clone = make_unique<Rectangle>();
		clone->SetPosition(pos_);
		clone->SetSize(size_);
		clone->SetTexture(texture_);
		
		return clone;
	}

	void SetPosition(Point pos) override
	{
		pos_ = pos;
	}

	void SetSize(Size size) override
	{
		size_ = size;
	}

	void SetTexture(shared_ptr<ITexture> tex_ptr) override
	{
		texture_ = tex_ptr;
	}

	Point GetPosition() const override
	{
		return pos_;
	}

	Size GetSize() const override
	{
		return size_;
	}

	ITexture* GetTexture() const override
	{
		return texture_.get();
	}

	void Draw(Image& image) const override
	{
		for (size_t i = pos_.y; i < image.size(); ++i) 
		{
			if (i - pos_.y < size_.height) 
			{
				for (size_t j = pos_.x; j < image[i].size(); ++j) 
				{
					if (j - pos_.x < size_.width) 
					{
						if (!texture_)
						{
							image[i][j] = '.';
						}
						else 
						{
							if ((i >= pos_.y && i < pos_.y + texture_->GetSize().height)
								&& (j >= pos_.x && j < pos_.x + texture_->GetSize().width)) 
							{
								image[i][j] = texture_->GetImage()[i - pos_.y][j - pos_.x];
							}
							else 
							{
								image[i][j] = '.';
							}
						}
					}
				}
			}
		}
	}

	~Rectangle() = default;

private:
	Size size_;
	shared_ptr<ITexture> texture_;
	Point pos_;
};


class Elipse : public IShape {
public:
	unique_ptr<IShape> Clone() const override
	{
		unique_ptr<IShape> clone = make_unique<Rectangle>();
		clone->SetPosition(pos_);
		clone->SetSize(size_);
		clone->SetTexture(texture_);

		return clone;
	}

	void SetPosition(Point pos) override
	{
		pos_ = pos;
	}

	void SetSize(Size size) override
	{
		size_ = size;
	}

	void SetTexture(shared_ptr<ITexture> tex_ptr) override
	{
		texture_ = tex_ptr;
	}

	Point GetPosition() const override
	{
		return pos_;
	}

	Size GetSize() const override
	{
		return size_;
	}

	ITexture* GetTexture() const override
	{
		return texture_.get();
	}

	void Draw(Image& image) const override
	{
		for (auto i = pos_.y; i < image.size(); ++i) 
		{
			if (i - pos_.y < size_.height) 
			{
				for (auto j = pos_.x; j < image[i].size(); ++j) 
				{
					if (j - pos_.x < size_.width) {
						if (texture_ == nullptr) {
							image[i][j] = '.';
						}
						else 
						{
							if (IsPointInEllipse({ j - pos_.x, i - pos_.y }, size_)) 
							{
								if ((i >= pos_.y && i < pos_.y + texture_->GetSize().height)
									&& (j >= pos_.x && j < pos_.x + texture_->GetSize().width)) 
								{
									image[i][j] = texture_->GetImage()[i - pos_.y][j - pos_.x];
								}
								else 
								{
									image[i][j] = '.';
								}
							}
						}
					}
				}
			}
		}
	}

	~Elipse() = default;

private:
	Size size_;
	shared_ptr<ITexture> texture_;
	Point pos_;
};


unique_ptr<IShape> MakeShape(ShapeType shape_type) {
	if (shape_type == ShapeType::Rectangle)
		return make_unique<Rectangle>();
	else
		return make_unique<Elipse>();
}