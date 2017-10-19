#include<cstdio>
#include<vector>
#include<cstring>
#include<algorithm>
using namespace std;
typedef unsigned int UInt;
typedef unsigned long long ULL;
class Number :public vector<UInt>
{
	bool flag;
	Number(UInt value){ flag = 0; if (value)push_back(value); }
public:
	int cmp(const Number& num)const;
	void add(const Number& num);
	void sub(const Number& num);
	Number mul(const Number& num)const;
	Number div(const Number& num, Number& mod)const;
	Number divInt(UInt num, UInt& mod)const;
	void shr(UInt num);
	void shl(UInt num);
	void shr31(UInt num);
	void shl31(UInt num);
	void and(const Number& num);
	void or(const Number& num);
	Number not(UInt k)const;
	void xor(const Number& num);
	static ULL link(ULL x, UInt y){ return (x << 31) | y; }
public:
	Number(){ flag = 0; }
	Number(int value);
	Number(long long value);
	Number(const char *s);
	Number(const char *s, UInt k);
	void convert10(char *s);
	void convert2k(char*s, UInt k);
	bool operator < (const Number& num)const;
	bool operator <= (const Number& num)const;
	bool operator == (const Number& num)const;
	Number operator + (const Number& num)const;
	Number operator - (const Number& num)const;
	Number operator  * (const Number& num)const;
	Number operator  / (const Number& num)const;
	Number operator % (const Number& num)const;
	Number operator >> (UInt num)const;
	Number operator << (UInt num)const;
	Number operator & (const Number& num)const;
	Number operator | (const Number& num)const;
	Number operator ^ (const Number& num)const;
};
Number::Number(const char *s)
{
	if (s[0] == '-'){ flag = 1; s++; }
	else flag = 0;
	vector<char> str(strlen(s));
	for (int i = str.size() - 1; i >= 0; i--)
		str[i] = s[str.size() - i - 1] - '0';
	while (str.size()){
		ULL sum = 0;
		for (int i = str.size() - 1; i >= 0; i--){
			sum = sum * 10 + str[i];
			str[i] = (char)(sum >> 31);
			sum &= ~(1 << 31);
		}
		push_back((UInt)sum);
		while (str.size() && !str.back())str.pop_back();
	}
	if (!back())pop_back();
}
Number::Number(const char *s, UInt k)
{
	if (s[0] == '-'){ flag = 1; s++; }
	else flag = 0;
	UInt cnt = 0;
	ULL value = 0;
	for (int i = strlen(s) - 1; i >= 0; i--){
		value |= (ULL)(s[i] <= '9' ? s[i] - '0' : s[i] - 'A' + 10) << cnt;
		cnt += k;
		if (cnt >= 31){
			push_back((UInt)value & ~(1 << 31));
			value >>= 31; cnt -= 31;
		}
	}
	if (value)push_back((UInt)value);
}
Number::Number(int value)
{
	if (value)push_back(value > 0 ? value : -value);
	flag = value < 0;
}
Number::Number(long long value)
{
	flag = value < 0;
	if (flag)value = -value;
	while (value){
		push_back(value & ~(1 << 31));
		value >>= 31;
	}
}
void Number::convert10(char *s)
{
	if (flag)*s++ = '-';
	vector<UInt> copy = *this;
	UInt len = 0;
	while (copy.size()){
		ULL sum = 0;
		for (int i = copy.size() - 1; i >= 0; i--){
			sum = link(sum, copy[i]);
			copy[i] = (UInt)(sum / 10);
			sum %= 10;
		}
		s[len++] = (char)sum + '0';
		if (!copy.back())copy.pop_back();
	}
	if (len == 0)s[len++] = '0';
	reverse(s, s + len);
	s[len] = 0;
}
void Number::convert2k(char*s, UInt k)
{
	const char *table = "0123456789ABCDEF";
	UInt len = 0, cnt = 0, bound = (1 << k) - 1;
	ULL value = 0;
	if (flag)*s++ = '-';
	if (empty())s[len++] = '0';
	else{
		for (UInt i = 0;; cnt -= k){
			if (cnt < k){
				if (i == size())break;
				value |= (ULL)(*this)[i++] << cnt;
				cnt += 31;
			}
			s[len++] = table[value & bound];
			value >>= k;
		}
		s[len++] = table[value];
		while (s[len - 1] == '0')len--;
		reverse(s, s + len);
	}
	s[len] = 0;
}
int Number::cmp(const Number& num)const
{
	if (size() != num.size())
		return size() < num.size() ? -1 : 1;
	for (int i = size() - 1; i >= 0; i--){
		if ((*this)[i] != num[i])
			return (*this)[i] < num[i] ? -1 : 1;
	}
	return 0;
}
bool Number::operator == (const Number& num)const{
	return flag == num.flag && !cmp(num);
}
bool Number::operator < (const Number& num)const{
	if (flag != num.flag)return flag;
	return flag ? cmp(num) > 0 : cmp(num) < 0;
}
bool Number::operator <= (const Number& num)const{
	if (flag != num.flag)return flag;
	return flag ? cmp(num) >= 0 : cmp(num) <= 0;
}
//为提高效率确保*this位数>=num位数
void Number::add(const Number& num)
{
	UInt f = 0, i = 0;
	if (size() < num.size())resize(num.size());
	for (; i < num.size(); i++){
		(*this)[i] += num[i] + f;
		f = (*this)[i] >> 31;
		if (f)(*this)[i] ^= 1 << 31;
	}
	push_back(0);
	for (; f; i++){
		f = ++(*this)[i] >> 31;
		if (f)(*this)[i] ^= 1 << 31;
	}
	if (!back())pop_back();
}
//确保*this>=num
void Number::sub(const Number& num)
{
	UInt f = 0, i = 0;
	for (; i < num.size(); i++){
		(*this)[i] -= num[i] + f;
		f = (*this)[i] >> 31;
		if (f)(*this)[i] ^= 1 << 31;
	}
	for (; f; i++){
		f = --(*this)[i] >> 31;
		if (f)(*this)[i] ^= 1 << 31;
	}
	while (size() && !back())pop_back();
}
Number Number::operator + (const Number& num)const
{
	Number ret;
	if (flag == num.flag){
		if (size() < num.size()){ ret = num; ret.add(*this); }
		else{ ret = *this; ret.add(num); }
		ret.flag = flag;
	}
	else{
		int t = cmp(num);
		if (t < 0){
			ret = num; ret.sub(*this);
			ret.flag = num.flag;
		}
		else if (t > 0){
			ret = *this; ret.sub(num);
			ret.flag = flag;
		}
	}
	return ret;
}
Number Number::operator - (const Number& num)const
{
	Number ret;
	if (flag != num.flag){
		if (size() < num.size()){ ret = num; ret.add(*this); }
		else{ ret = *this; ret.add(num); }
		ret.flag = flag;
	}
	else{
		int t = cmp(num);
		if (t < 0){
			ret = num; ret.sub(*this);
			ret.flag = !flag;
		}
		else if (t > 0){
			ret = *this; ret.sub(num);
			ret.flag = flag;
		}
	}
	return ret;
}
//为提高效率确保*this位数>=num位数
Number Number::mul(const Number& num)const
{
	if (num.empty() || empty())return 0;
	Number ret;
	ret.resize(size() + num.size(), 0);
	for (int i = num.size() - 1; i >= 0; i--){
		ULL sum = 0;
		for (UInt j = 0; j < size(); j++){
			sum += (ULL)num[i] * (*this)[j];
			ret[i + j] += sum & ~(1 << 31);
			sum >>= 31;
			if (ret[i + j] & (1 << 31)){
				sum++;
				ret[i + j] ^= 1 << 31;
			}
		}
		ret[i + size()] += (UInt)sum;
	}
	for (UInt i = size(); i < ret.size(); i++){
		if (ret[i] & (1 << 31)){
			ret[i] ^= 1 << 31;
			ret[i + 1]++;
		}
	}
	if (!ret.back())ret.pop_back();
	return ret;
}
Number Number::operator * (const Number& num)const
{
	Number ret = size() < num.size() ? num.mul(*this) : mul(num);
	if (ret.size())ret.flag = flag ^ num.flag;
	return ret;
}
Number Number::div(const Number& num, Number& mod)const
{
	const UInt aSize = size(), bSize = num.size();
	if (aSize < bSize){ mod = *this; return 0; }
	Number ret;
	ret.resize(aSize - bSize + 1);
	mod.assign(begin() + aSize - bSize + 1, end());
	ULL y = num.back();
	int bit = 0;
	for (int i = 16; i; i >>= 1){
		if (y >> (bit + i))bit += i;
	}
	y = (y << (31 - bit)) + (num[bSize - 2] >> bit) + 1;
	for (int i = ret.size() - 1; i >= 0; i--){
		mod.shl31(1);
		UInt oldSize = mod.size();
		mod.resize(bSize + 1);
		mod[0] = (*this)[i];
		ULL x = link(mod[bSize], mod[bSize - 1]);
		x = (x << (31 - bit)) | (mod[bSize - 2] >> bit);
		if (!oldSize && mod[0])oldSize++;
		mod.resize(oldSize);
		if (ret[i] = (UInt)(x / y))mod.sub(num.mul(ret[i]));
		if (mod.cmp(num) >= 0){
			mod.sub(num);
			ret[i]++;
		}
	}
	if (!ret.back())ret.pop_back();
	return ret;
}
Number Number::divInt(UInt num, UInt& mod)const
{
	Number ret;
	ret.resize(size());
	ULL sum = 0;
	for (int i = size() - 1; i >= 0; i--){
		sum = link(sum, (*this)[i]);
		ret[i] = (UInt)(sum / num);
		sum %= num;
	}
	if (ret.size() && !ret.back())ret.pop_back();
	mod = (UInt)sum;
	return ret;
}
Number Number::operator / (const Number& num)const
{
	UInt t;
	Number ret = num.size() == 1 ? divInt(num[0], t) : div(num, Number());
	if (ret.size())ret.flag = flag ^ num.flag;
	return ret;
}
Number Number::operator % (const Number& num)const
{
	Number ret;
	if (num.size() == 1){
		UInt t;
		divInt(num[0], t);
		ret = t;
	}
	else div(num, ret);
	if (ret.size())ret.flag = flag;
	return ret;
}
void Number::shr(UInt num)
{
	if (!num)return;
	UInt t = num / 31, k = num % 31;
	if (size() <= t)clear();
	else{
		UInt newSize = size() - t;
		for (UInt i = 0; i < newSize - 1; i++)
			(*this)[i] = ((*this)[i + t] >> k) | ((*this)[i + t + 1] << (31 - k)) & 0x7fffffff;
		(*this)[newSize - 1] = back() >> k;
		resize(newSize);
		if (!back())pop_back();
	}
}
void Number::shr31(UInt num)
{
	if (size() <= num)clear();
	else{
		UInt newSize = size() - num;
		for (UInt i = 0; i < newSize; i++)
			(*this)[i] = (*this)[i + num];
		resize(newSize);
	}
}
void Number::shl(UInt num)
{
	if (empty() || !num)return;
	UInt t = (num + 30) / 31, k = (num + 30) % 31 + 1;
	UInt oldSize = size();
	resize(oldSize + t);
	for (int i = oldSize - 1; i >= 0; i--)
		(*this)[i + t] = ((*this)[i + 1] << k) | ((*this)[i] >> (31 - k)) & 0x7fffffff;
	(*this)[t - 1] = (front() << k) & 0x7fffffff;
	for (int i = t - 2; i >= 0; i--)
		(*this)[i] = 0;
	if (!back())pop_back();
}
void Number::shl31(UInt num)
{
	if (empty())return;
	UInt oldSize = size();
	resize(oldSize + num);
	for (int i = oldSize - 1; i >= 0; i--)
		(*this)[i + num] = (*this)[i];
	for (int i = num - 1; i >= 0; i--)
		(*this)[i] = 0;
}
Number Number::operator >> (UInt num)const
{
	bool f = false;
	Number ret = *this;
	if (flag){
		UInt i, t;
		for (i = 0; !(*this)[i]; i++);
		t = i * 31;
		f = t < num && ((t + 31 <= num) || ((1 << (num - t)) - 1) & (*this)[i]);
	}
	ret.shr(num);
	if (f)ret.add(1);
	return ret;
}
Number Number::operator << (UInt num)const
{
	Number ret = *this;
	ret.shl(num);
	return ret;
}
void Number::and(const Number& num)
{
	if (size() > num.size())resize(num.size());
	for (UInt i = 0; i < size(); i++)
		(*this)[i] &= num[i];
	while (size() && !back())pop_back();
}
//为提高效率确保*this位数>=num位数
void Number::or(const Number& num)
{
	if (size() < num.size())resize(num.size());
	for (UInt i = 0; i < num.size(); i++)
		(*this)[i] |= num[i];
}
Number Number::not(UInt k)const
{
	Number ret;
	ret.resize(k, ~(1 << 31));
	for (int i = min(size(), k) - 1; i >= 0; i--)
		ret[i] = (*this)[i] ^ ~(1 << 31);
	while (ret.size() && !ret.back())ret.pop_back();
	return ret;
}
//为提高效率确保*this位数>=num位数
void Number::xor(const Number& num)
{
	if (size() < num.size())resize(num.size());
	for (UInt i = 0; i < num.size(); i++)
		(*this)[i] ^= num[i];
	while (size() && !back())pop_back();
}
Number Number::operator & (const Number& num)const
{
	Number ret;
	if (flag != num.flag){
		if (flag){
			ret = not(num.size()); ret.add(1);
			ret.and(num);
		}
		else{
			ret = num.not(size()); ret.add(1);
			ret.and(*this);
		}
	}
	else if (flag){
		Number temp;
		if (size() < num.size()){ ret = num; temp = *this; }
		else{ ret = *this; temp = num; }
		ret.sub(1); temp.sub(1);
		ret.or(temp); ret.add(1);
	}
	else if (size() < num.size()){ ret = *this; ret.and(num); }
	else{ ret = num; ret.and(*this); }
	ret.flag = flag & num.flag;
	return ret;
}
Number Number::operator | (const Number& num)const
{
	Number ret;
	if (flag != num.flag){
		Number temp;
		if (flag){ ret = num.not(size()); temp = *this; }
		else{ ret = not(num.size()); temp = num; }
		temp.sub(1);
		ret.and(temp); ret.add(1);
	}
	else if (flag){
		Number temp;
		if (size() < num.size()){ ret = *this; temp = num; }
		else{ ret = num; temp = *this; }
		ret.sub(1); temp.sub(1);
		ret.and(temp); ret.add(1);
	}
	else if (size() < num.size()){ ret = num; ret.or(*this); }
	else{ ret = *this; ret.or(num); }
	ret.flag = flag | num.flag;
	return ret;
}
Number Number::operator ^ (const Number& num)const
{
	Number ret;
	if (flag != num.flag){
		if (flag){
			ret = *this; ret.sub(1);
			ret.xor(num);
		}
		else{
			ret = num; ret.sub(1);
			ret.xor(*this);
		}
		ret.add(1);
	}
	else if(flag){
		Number temp;
		if (size() < num.size()){ ret = num; temp = *this; }
		else{ ret = *this; temp = num; }
		ret.sub(1); temp.sub(1);
		ret.xor(temp);
	}
	else if (size() < num.size()){ ret = num; ret.xor(*this); }
	else{ ret = *this; ret.xor(num); }
	ret.flag = flag ^ num.flag;
	return ret;
}
