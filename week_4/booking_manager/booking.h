namespace RAII {
	template<typename Provider>
	class Booking
	{
	public:
		Booking(Provider* provider, int count)
			: provider_(provider), count_books_(count)
		{
		}

		Booking(Booking<Provider>&& other)
		{
			provider_ = other.provider_;
			other.provider_ = nullptr;
		}

		Booking<Provider>& operator= (Booking<Provider>&& other)
		{
			delete provider_;
			provider_ = other.provider_;
			other.provider_ = nullptr;

			return *this;
		}

		~Booking()
		{
			if (provider_)
				provider_->CancelOrComplete(*this);
		}

	private:
		Provider* provider_;
		int count_books_;
	};
}