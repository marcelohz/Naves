#define SCREENX 600
#define SCREENY 600

class CMe
{
public:
	long Width;
	long Height;
	float PosX;
	float PosY;
	CMe(void)
	{
		this->Height = 30;
		this->Width = 30;
		this->PosX = SCREENX / 2.0;
		this->PosY = (float)(SCREENY - Width*2);
	}
};