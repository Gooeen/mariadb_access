//*********************************************************
// CopyRight (C) 2015 Gooeen
// All rights reserved
//
// �ļ����� : mariadb.cpp
//
// ��ǰ�汾 : 1.0.1
// ��    �� : Gooeen
// ������� : 2015/09/12
//
// ժ    Ҫ : ���ڷ��� MySQL/MariaDB �����
//*********************************************************
#include "mariadb.h"
#include <cassert>


// ��������� false ���׳��쳣
#define if_false_throw(condition, message)\
if (!condition)\
{\
	throw mariadb_exception(this->error() + "\r\nSQL: " + message, __FILE__, __LINE__);\
}


//*********************************************************
// �������� : to_date_string
// ��    �� : Gooeen
// ������� : 2015/09/12
// ����˵�� : ������ת���ɷ��� MySQL/MariaDB ��ʽ���ַ���
// ���ʷ�ʽ : public
// �������� : unsigned int year ���
// �������� : unsigned int month �·�
// �������� : unsigned int day ����
// �� �� ֵ : std::string ���� MySQL/MariaDB ��ʽ�������ַ���
// ��    �� : ������Է���ռ�ʧ�����׳� std::bad_alloc �쳣
//*********************************************************
std::string sql::mariadb::to_date_string(unsigned int year, unsigned int month, unsigned int day)
{
	const auto y = std::to_string(year);
	const auto m = std::to_string(month);
	const auto d = std::to_string(day);
	return y + "-" + m + "-" + d;
}


//*********************************************************
// �������� : to_time_string
// ��    �� : Gooeen
// ������� : 2015/09/12
// ����˵�� : ��ʱ��ת���ɷ��� MySQL/MariaDB ��ʽ���ַ���
// ���ʷ�ʽ : public
// �������� : unsigned int hour ʱ
// �������� : unsigned int minute ��
// �������� : unsigned int second ��
// �� �� ֵ : std::string ���� MySQL/MariaDB ��ʽ��ʱ���ַ���
// ��    �� : ������Է���ռ�ʧ�����׳� std::bad_alloc �쳣
//*********************************************************
std::string sql::mariadb::to_time_string(unsigned int hour, unsigned int minute, unsigned int second)
{
	const auto h = std::to_string(hour);
	const auto m = std::to_string(minute);
	const auto s = std::to_string(second);
	return h + "-" + m + "-" + s;
}


//*********************************************************
// �������� : to_datetime_string
// ��    �� : Gooeen
// ������� : 2015/09/12
// ����˵�� : ������ʱ��ת���ɷ��� MySQL/MariaDB ��ʽ���ַ���
// ���ʷ�ʽ : public
// �������� : unsigned int year ��
// �������� : unsigned int month ��
// �������� : unsigned int day ��
// �������� : unsigned int hour ʱ
// �������� : unsigned int minute ��
// �������� : unsigned int second ��
// �� �� ֵ : std::string ���� MySQL/MariaDB ��ʽ������ʱ���ַ���
// ��    �� : ������Է���ռ�ʧ�����׳� std::bad_alloc �쳣
//*********************************************************
std::string sql::mariadb::to_datetime_string(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second)
{
	const auto d = to_date_string(year, month, day);
	const auto t = to_time_string(hour, minute, second);
	return d + " " + t;
}


//*********************************************************
// �������� : mariadb_exception
// ��    �� : Gooeen
// ������� : 2015/09/12
// ����˵�� : ���캯��
// ���ʷ�ʽ : public
// �������� : const std::string & text �쳣��Ϣ
// �������� : const std::string & file �쳣�����ļ�
// �������� : int line �쳣������
// ��    �� : ����쳣��Ϣ���� (length) �����ַ���
//            ��󳤶�(max_size) ���׳� std::length_error �쳣;
//            ������Է���ռ�ʧ�����׳� std::bad_alloc �쳣
//*********************************************************
sql::mariadb::mariadb_exception::mariadb_exception(const std::string & text, const std::string & file, int line)
	: m_text(text + "\r\nin " + file + ", row: " + std::to_string(line))
{
}


//*********************************************************
// �������� : what
// ��    �� : Gooeen
// ������� : 2015/09/12
// ����˵�� : ����쳣��Ϣ
// ���ʷ�ʽ : public
// �� �� ֵ : const char * �쳣��Ϣ�ַ���
//*********************************************************
const char * sql::mariadb::mariadb_exception::what() const
{
	return m_text.c_str();
}


//*********************************************************
// �������� : connection
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ���캯��
// ���ʷ�ʽ : public
//*********************************************************
sql::mariadb::connection::connection(void) noexcept
	: m_ptr_mysql(mysql_init(nullptr))
{
}


//*********************************************************
// �������� : connection
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ת�ƹ��캯��, ת�ƺ�ԭ����Ӧ��ʹ��
// ���ʷ�ʽ : public
// �������� : connection && connector ��Ҫת�ƵĶ���
//*********************************************************
sql::mariadb::connection::connection(connection && connector) noexcept
	: m_ptr_mysql(connector.m_ptr_mysql)
{
	connector.m_ptr_mysql = nullptr;
}


//*********************************************************
// �������� : ~connection
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ������ݿ�����û�йر���������ʱ�Զ��ر�
// ���ʷ�ʽ : public
//*********************************************************
sql::mariadb::connection::~connection(void) noexcept
{
	if (m_ptr_mysql != nullptr)
	{
		mysql_close(m_ptr_mysql);
	}
}


//*********************************************************
// �������� : operator!=
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : �ж����ݿ��������Ƿ񴴽��ɹ�
// ���ʷ�ʽ : public
// �������� : std::nullptr_t null ��nullptr���Ƚ�
// �� �� ֵ : bool ��������ɹ��� ���� != nullptr;
//            ��֮ ���� == nullptr
//*********************************************************
bool sql::mariadb::connection::operator!=(std::nullptr_t null) const noexcept
{
	return m_ptr_mysql != null;
}


//*********************************************************
// �������� : operator==
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : �ж����ݿ��������Ƿ񴴽��ɹ�
// ���ʷ�ʽ : public
// �������� : std::nullptr_t null  ��nullptr���Ƚ�
// �� �� ֵ : bool ��������ɹ��� ���� != nullptr;
//            ��֮ ���� == nullptr
//*********************************************************
bool sql::mariadb::connection::operator==(std::nullptr_t null) const noexcept
{
	return m_ptr_mysql == null;
}


//*********************************************************
// �������� : open
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : �������ݿ�
// ���ʷ�ʽ : public
// �������� : const char * user �û���
// �������� : const char * password ����
// �������� : const char * database ���ݿ���
// �������� : unsigned int port �������˿�
// �������� : const char * host ������ IP ��ַ
// �������� : const char * unix_socket ָ����Ҫʹ�õ� socket ���� �����ܵ�
// �������� : unsigned long flags ����ѡ��
// �� �� ֵ : bool true �������ӳɹ�, false ��������ʧ��
//*********************************************************
bool sql::mariadb::connection::open(const char * user, const char * password, const char * database, unsigned int port, const char * host, const char * unix_socket, unsigned long flags) const noexcept
{
	assert(host != nullptr);
	assert(user != nullptr);
	assert(password != nullptr);

	if (m_ptr_mysql == nullptr)
	{
		return false;
	}

	bool result = mysql_real_connect(m_ptr_mysql, host, user, password, database, port, unix_socket, flags) != nullptr;
	mysql_set_character_set(m_ptr_mysql, "utf8");

	return result;
}


//*********************************************************
// �������� : close
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : �ر����ݿ�����, ������ر����ݿ�����, ������ʱ�Զ��ر�
// ���ʷ�ʽ : public
//*********************************************************
void sql::mariadb::connection::close(void) noexcept
{
	if (m_ptr_mysql != nullptr)
	{
		mysql_close(m_ptr_mysql);
		m_ptr_mysql = nullptr;
	}
}


//*********************************************************
// �������� : error
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ�����Ϣ
// ���ʷ�ʽ : public
// �� �� ֵ : std::string ������Ϣ
//*********************************************************
std::string sql::mariadb::connection::error(void) const noexcept
try
{
	return std::string(mysql_error(m_ptr_mysql));
}
catch (const std::exception &)
{
	return std::string();
}


//*********************************************************
// �������� : errorno
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ������
// ���ʷ�ʽ : public
// �� �� ֵ : unsigned int �������
//*********************************************************
unsigned int sql::mariadb::connection::errorno(void) const noexcept
{
	return mysql_errno(m_ptr_mysql);
}


//*********************************************************
// �������� : recordset
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ת�ƹ��캯��, ת�ƺ�ԭ����Ӧ��ʹ��
// ���ʷ�ʽ : private
// �������� : recordset && reader ��Ҫת�ƵĶ���
//*********************************************************
sql::mariadb::recordset::recordset(recordset && reader) noexcept
	: m_ptr_mysql(reader.m_ptr_mysql)
	, m_ptr_res(reader.m_ptr_res)
	, m_row(reader.m_row)
{
	reader.m_ptr_mysql = nullptr;
	reader.m_ptr_res = nullptr;
	reader.m_row = nullptr;
}


//*********************************************************
// �������� : ~recordset
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��������û���ͷŹر���������ʱ�Զ��ر�
// ���ʷ�ʽ : public
//*********************************************************
sql::mariadb::recordset::~recordset(void) noexcept
{
	if (m_ptr_res != nullptr)
	{
		mysql_free_result(m_ptr_res);
	}
}


//*********************************************************
// �������� : operator!=
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : �ж����ݿ��������Ƿ񴴽��ɹ�
// ���ʷ�ʽ : public
// �������� : std::nullptr_t null ��nullptr���Ƚ�
// �� �� ֵ : bool ��������ɹ��� ���� != nullptr;
//            ��֮ ���� == nullptr
//*********************************************************
bool sql::mariadb::recordset::operator!=(std::nullptr_t null) const noexcept
{
	return m_ptr_res != null;
}


//*********************************************************
// �������� : operator==
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : �ж����ݿ��������Ƿ񴴽��ɹ�
// ���ʷ�ʽ : public
// �������� : std::nullptr_t null  ��nullptr���Ƚ�
// �� �� ֵ : bool ��������ɹ��� ���� != nullptr;
//            ��֮ ���� == nullptr
//*********************************************************
bool sql::mariadb::recordset::operator==(std::nullptr_t null) const noexcept
{
	return m_ptr_res == null;
}


//*********************************************************
// �������� : close
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : �ͷŽ������ռ�õ���Դ, ������ֶ��ͷ���Դ, ������ʱ�Զ��ͷ�
// ���ʷ�ʽ : public
//*********************************************************
void sql::mariadb::recordset::close(void) noexcept
{
	if (m_ptr_res != nullptr)
	{
		mysql_free_result(m_ptr_res);
		m_ptr_res = nullptr;
		m_ptr_mysql = nullptr;
	}
}


//*********************************************************
// �������� : error
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ�����Ϣ
// ���ʷ�ʽ : public
// �� �� ֵ : std::string ������Ϣ
//*********************************************************
std::string sql::mariadb::recordset::error(void) const noexcept
try
{
	return std::string(mysql_error(m_ptr_mysql));
}
catch (const std::exception &)
{
	return std::string();
}


//*********************************************************
// �������� : errorno
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ������
// ���ʷ�ʽ : public
// �� �� ֵ : unsigned int �������
//*********************************************************
unsigned int sql::mariadb::recordset::errorno(void) const noexcept
{
	return mysql_errno(m_ptr_mysql);
}


//*********************************************************
// �������� : row_count
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ���ݵ�����
// ���ʷ�ʽ : public
// �� �� ֵ : unsigned long long ����
//*********************************************************
unsigned long long sql::mariadb::recordset::row_count(void) const noexcept
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	return mysql_num_rows(m_ptr_res);
}


//*********************************************************
// �������� : field_count
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ���ݵ�����(�ֶ�����)
// ���ʷ�ʽ : public
// �� �� ֵ : unsigned long ����(�ֶ�����)
//*********************************************************
unsigned long sql::mariadb::recordset::field_count(void) const noexcept
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	return mysql_num_fields(m_ptr_res);
}


//*********************************************************
// �������� : data_size
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ�������ݵĳ���
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��; ����������ݵ���������ܳ�������Խ�������
// �� �� ֵ : unsigned long �������ݵ��ֽ���
//*********************************************************
unsigned long sql::mariadb::recordset::data_size(unsigned long n) const noexcept
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return mysql_fetch_lengths(m_ptr_res)[n];
}


//*********************************************************
// �������� : read
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ��һ������
// ���ʷ�ʽ : public
// �� �� ֵ : bool �ɹ�����true, ʧ�ܷ���false
//*********************************************************
bool sql::mariadb::recordset::read(void) noexcept
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	m_row = mysql_fetch_row(m_ptr_res);
	return m_row != nullptr;
}


//*********************************************************
// �������� : get_char
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ��������ǿ��ת���� char ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : char ����
//*********************************************************
char sql::mariadb::recordset::get_char(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return m_row[n][0];
}


//*********************************************************
// �������� : get_udata
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ���������� std::vector<unsigned char> ���󱣴�
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : std::vector<unsigned char> ����
// ��    �� : ��� std::vector ����ʧ��ʱ�׳��쳣
//*********************************************************
std::vector<unsigned char> sql::mariadb::recordset::get_udata(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::vector<unsigned char>(m_row[n], m_row[n] + this->data_size(n));
}


//*********************************************************
// �������� : get_data
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ���������� std::vector<char> ���󱣴�
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : std::vector<char> ����
// ��    �� : ��� std::vector ����ʧ��ʱ�׳��쳣
//*********************************************************
std::vector<char> sql::mariadb::recordset::get_data(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::vector<char>(m_row[n], m_row[n] + this->data_size(n));
}


//*********************************************************
// �������� : get_double
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ��������ת���� double ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : double ����
//*********************************************************
double sql::mariadb::recordset::get_double(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::atof(m_row[n]);
}


//*********************************************************
// �������� : get_float
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ��������ת���� float ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : float ����
//*********************************************************
float sql::mariadb::recordset::get_float(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return (float)std::atof(m_row[n]);
}


//*********************************************************
// �������� : get_int
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ��������ת���� int ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : int ����
//*********************************************************
int sql::mariadb::recordset::get_int(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::atoi(m_row[n]);
}


//*********************************************************
// �������� : get_long
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ��������ת���� long ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : long ����
//*********************************************************
long sql::mariadb::recordset::get_long(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::atol(m_row[n]);
}


//*********************************************************
// �������� : get_longlong
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ��������ת���� long long ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : long long ����
//*********************************************************
long long sql::mariadb::recordset::get_longlong(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::atoll(m_row[n]);
}


//*********************************************************
// �������� : get_short
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ��������ת���� short ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : short ����
//*********************************************************
short sql::mariadb::recordset::get_short(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return (short)std::atoi(m_row[n]);
}


//*********************************************************
// �������� : get_string
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : std::string ����
// ��    �� : ������ݳ��� (length) �����ַ�����󳤶�(max_size)
//            ���׳� std::length_error �쳣;
//            ������Է���ռ�ʧ�����׳� std::bad_alloc �쳣
//*********************************************************
std::string sql::mariadb::recordset::get_string(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::string(m_row[n]);
}


//*********************************************************
// �������� : get_uchar
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ��������ǿ��ת���� unsigned char ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : unsigned char ����
//*********************************************************
unsigned char sql::mariadb::recordset::get_uchar(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return (unsigned char)m_row[n][0];
}


//*********************************************************
// �������� : get_uint
// ��    �� : Gooeen
// ������� : 2015/09/13
// ����˵�� : ��ȡ����, ��������ת���� unsigned int ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : unsigned int ����
// ��    �� : ���ǿ��ת��ʧ��, ���׳� std::invalid_argument �쳣;
//            �������ֵ���� unsigned int ��ȡֵ��Χ, ���׳� std::out_of_range �쳣
//*********************************************************
unsigned int sql::mariadb::recordset::get_uint(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return (unsigned int)std::stoul(m_row[n]);
}


//*********************************************************
// �������� : get_ulong
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ��ȡ����, ��������ת���� unsigned long ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : unsigned long ����
// ��    �� : ���ǿ��ת��ʧ��, ���׳� std::invalid_argument �쳣;
//            �������ֵ���� unsigned long ��ȡֵ��Χ, ���׳� std::out_of_range �쳣
//*********************************************************
unsigned long sql::mariadb::recordset::get_ulong(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::stoul(m_row[n]);
}


//*********************************************************
// �������� : get_ulonglong
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ��ȡ����, ��������ת���� unsigned long long ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : unsigned long long ����
// ��    �� : ���ǿ��ת��ʧ��, ���׳� std::invalid_argument �쳣;
//            �������ֵ���� unsigned long long ��ȡֵ��Χ, ���׳� std::out_of_range �쳣
//*********************************************************
unsigned long long sql::mariadb::recordset::get_ulonglong(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::stoull(m_row[n]);
}


//*********************************************************
// �������� : get_ushort
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ��ȡ����, ��������ת���� unsigned short ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : unsigned short ����
//*********************************************************
unsigned short sql::mariadb::recordset::get_ushort(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return (unsigned short)std::atoi(m_row[n]);
}


//*********************************************************
// �������� : get_wchar
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ��ȡ����, ��������ǿ��ת���� wchar_t ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : wchar_t ����
//*********************************************************
wchar_t sql::mariadb::recordset::get_wchar(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return *((wchar_t *)m_row[n]);
}


//*********************************************************
// �������� : get_wstring
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ��ȡ����, ��������ǿ��ת���� std::wstring ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : std::wstring ����
//*********************************************************
std::wstring sql::mariadb::recordset::get_wstring(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return std::wstring((wchar_t *)m_row[n], mysql_fetch_lengths(m_ptr_res)[n] / 2);
}


//*********************************************************
// �������� : get_raw
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ������ת��, ֱ�ӻ�ȡ�����ݿⷵ�ص�����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : const char * ����
//*********************************************************
const char * sql::mariadb::recordset::get_raw(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return m_row[n];
}


//*********************************************************
// �������� : get_bool
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ��ȡ����, ��������ǿ��ת���� bool ����
// ���ʷ�ʽ : public
// �������� : unsigned long n ���ݵ���λ��
// �� �� ֵ : bool ����
//*********************************************************
bool sql::mariadb::recordset::get_bool(unsigned long n) const
{
	assert(m_ptr_mysql != nullptr);
	assert(m_ptr_res != nullptr);
	assert(m_row != nullptr);
	assert(n < mysql_num_fields(m_ptr_res));
	return (*m_row[n]) - '0' != 0;
}


//*********************************************************
// �������� : recordset
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : �� command ����󴴽� recordset �����
// ���ʷ�ʽ : private
// �������� : MYSQL * pointer ���ݿ���
//*********************************************************
sql::mariadb::recordset::recordset(MYSQL * pointer) noexcept
	: m_ptr_mysql(pointer)
	, m_ptr_res(mysql_store_result(pointer))
	, m_row(nullptr)
{
}


//*********************************************************
// �������� : command
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ת�ƹ��캯��, ת�ƺ�ԭ����Ӧ��ʹ��
// ���ʷ�ʽ : public
// �������� : command && executor ��Ҫת�ƵĶ���
//*********************************************************
sql::mariadb::command::command(command && executor) noexcept
	: m_ptr_mysql(executor.m_ptr_mysql)
	, m_text(std::move(executor.m_text))
	, m_strings(std::move(executor.m_strings))
	, m_datas(std::move(executor.m_datas))
	, m_udatas(std::move(executor.m_udatas))
	, m_parameters(std::move(executor.m_parameters))
{
	executor.m_ptr_mysql = nullptr;
}


//*********************************************************
// �������� : command
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ���캯��
// ���ʷ�ʽ : public
// �������� : const connection & connector ���ݿ�������
//*********************************************************
sql::mariadb::command::command(const connection & connector) noexcept
	: m_ptr_mysql(connector.m_ptr_mysql)
{
}


//*********************************************************
// �������� : command
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ���캯��
// ���ʷ�ʽ : public
// �������� : const connection & connector ���ݿ�������
// �������� : const std::string & text SQL���
// ��    �� : ������Է���ռ�ʧ�����׳� std::bad_alloc �쳣
//*********************************************************
sql::mariadb::command::command(const connection & connector, const std::string & text)
	: command(connector, std::string(text))
{
}


//*********************************************************
// �������� : command
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ���캯��
// ���ʷ�ʽ : public
// �������� : const connection & connector ���ݿ�������
// �������� : std::string && text SQL���
//*********************************************************
sql::mariadb::command::command(const connection & connector, std::string && text) noexcept
	: m_ptr_mysql(connector.m_ptr_mysql)
	, m_text(std::move(text))
{
}


//*********************************************************
// �������� : error
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ�����Ϣ
// ���ʷ�ʽ : public 
// �������� : void
// �� �� ֵ : std::string ������Ϣ
//*********************************************************
std::string sql::mariadb::command::error(void) const noexcept
try
{
	return std::string(mysql_error(m_ptr_mysql));
}
catch (const std::exception &)
{
	return std::string();
}


//*********************************************************
// �������� : errorno
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ������
// ���ʷ�ʽ : public
// �� �� ֵ : unsigned int �������
//*********************************************************
unsigned int sql::mariadb::command::errorno(void) const noexcept
{
	return mysql_errno(m_ptr_mysql);
}


//*********************************************************
// �������� : prepare
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : Ԥ��SQL���; ��ʹ���������ӷ�ע��
//            executer.prepare("select * from table where id = ?");
//            executer.add(12);
// ���ʷ�ʽ : public
// �������� : std::string && text
// �� �� ֵ : void
//*********************************************************
void sql::mariadb::command::prepare(std::string && text) noexcept
{
	m_text = std::move(text);
}


//*********************************************************
// �������� : prepare
// ��    �� : Gooeen
// ������� : 2015/09/15
// ����˵�� : Ԥ��SQL���; ��ʹ���������ӷ�ע��
// ���ʷ�ʽ : public
// �������� : const std::string & text SQL���
// �� �� ֵ : void
// ��    �� : ������Է���ռ�ʧ�����׳� std::bad_alloc �쳣
//*********************************************************
void sql::mariadb::command::prepare(const std::string & text)
{
	m_text = text;
}


//*********************************************************
// �������� : add
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : �������, ִ��SQL���ʱ��������ӵ������õ���
//            ����������SQL����е� pos ���ʺ�; ��������:
//            executer.prepare("insert into table1 values(?, ?)");
//            executer.add(0, 12);
//            executer.add(1, "data");
//            ��ִ��SQL���ʱ������ SQL���:
//            insert into table1 values(12, 'data')
// ���ʷ�ʽ : public
// �������� : unsigned int pos ��Ҫ�����ʺŵ�λ��, ��0��ʼ
// �������� : const std::string & text ����, ��������SQL����н��Ե���������
// �� �� ֵ : void
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
//*********************************************************
void sql::mariadb::command::add(unsigned int pos, const std::string & text)
{
	this->add(pos, std::string(text));
}


//*********************************************************
// �������� : add
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : �������, ִ��SQL���ʱ��������ӵ������õ���
//            ����������SQL����е� pos ���ʺ�; ��������:
//            executer.prepare("insert into table1 values(?, ?)");
//            executer.add(0, 12);
//            executer.add(1, "data");
//            ��ִ��SQL���ʱ������ SQL���:
//            insert into table1 values(12, 'data')
// ���ʷ�ʽ : public
// �������� : unsigned int pos ��Ҫ�����ʺŵ�λ��, ��0��ʼ
// �������� : const std::string & text ����, ��������SQL����н��Ե���������
// �� �� ֵ : void
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
//*********************************************************
void sql::mariadb::command::add(unsigned int pos, std::string && text)
{
	// �����ַ���
	m_strings.push_back(std::move(text));

	// �������ݵ���Ϣ
	const auto p = m_strings.back().c_str();
	const auto size = m_strings.back().size();
	const auto pair = std::make_pair(p, size);
	m_parameters[pos] = std::make_pair(false, pair);
}


//*********************************************************
// �������� : add
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : �������, ִ��SQL���ʱ��������ӵ������õ���
//            ����������SQL����е� pos ���ʺ�; ��������:
//            executer.prepare("insert into table1 values(?, ?)");
//            executer.add(0, 12);
//            executer.add(1, "data");
//            ��ִ��SQL���ʱ������ SQL���:
//            insert into table1 values(12, 'data')
// ���ʷ�ʽ : public
// �������� : unsigned int pos ��Ҫ�����ʺŵ�λ��, ��0��ʼ
// �������� : const std::string & text ����, ��������SQL����н��Ե���������
// �� �� ֵ : void
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
//*********************************************************
void sql::mariadb::command::add(unsigned int pos, const std::vector<char> &data)
{
	this->add(pos, std::vector<char>(data));
}


//*********************************************************
// �������� : add
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : �������, ִ��SQL���ʱ��������ӵ������õ���
//            ����������SQL����е� pos ���ʺ�; ��������:
//            executer.prepare("insert into table1 values(?, ?)");
//            executer.add(0, 12);
//            executer.add(1, "data");
//            ��ִ��SQL���ʱ������ SQL���:
//            insert into table1 values(12, 'data')
// ���ʷ�ʽ : public
// �������� : unsigned int pos ��Ҫ�����ʺŵ�λ��, ��0��ʼ
// �������� : const std::string & text ����, ��������SQL����н��Ե���������
// �� �� ֵ : void
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
//*********************************************************
void sql::mariadb::command::add(unsigned int pos, std::vector<char> &&data)
{
	// ��������
	m_datas.push_back(std::move(data));

	// �������ݵ���Ϣ
	const auto p = m_datas.back().data();
	const auto size = m_datas.back().size();
	const auto pair = std::make_pair(p, size);
	m_parameters[pos] = std::make_pair(false, pair);
}


//*********************************************************
// �������� : add
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : �������, ִ��SQL���ʱ��������ӵ������õ���
//            ����������SQL����е� pos ���ʺ�; ��������:
//            executer.prepare("insert into table1 values(?, ?)");
//            executer.add(0, 12);
//            executer.add(1, "data");
//            ��ִ��SQL���ʱ������ SQL���:
//            insert into table1 values(12, 'data')
// ���ʷ�ʽ : public
// �������� : unsigned int pos ��Ҫ�����ʺŵ�λ��, ��0��ʼ
// �������� : const std::string & text ����, ��������SQL����н��Ե���������
// �� �� ֵ : void
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
//*********************************************************
void sql::mariadb::command::add(unsigned int pos, const std::vector<unsigned char> &data)
{
	this->add(pos, std::vector<unsigned char>(data));
}


//*********************************************************
// �������� : add
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : �������, ִ��SQL���ʱ��������ӵ������õ���
//            ����������SQL����е� pos ���ʺ�; ��������:
//            executer.prepare("insert into table1 values(?, ?)");
//            executer.add(0, 12);
//            executer.add(1, "data");
//            ��ִ��SQL���ʱ������ SQL���:
//            insert into table1 values(12, 'data')
// ���ʷ�ʽ : public
// �������� : unsigned int pos ��Ҫ�����ʺŵ�λ��, ��0��ʼ
// �������� : const std::string & text ����, ��������SQL����н��Ե���������
// �� �� ֵ : void
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
//*********************************************************
void sql::mariadb::command::add(unsigned int pos, std::vector<unsigned char> &&data)
{
	// ��������
	m_udatas.push_back(std::move(data));

	// �������ݵ���Ϣ
	const auto p = (char *)m_udatas.back().data();
	const auto size = m_udatas.back().size();
	const auto pair = std::make_pair(p, size);
	m_parameters[pos] = std::make_pair(false, pair);
}


//*********************************************************
// �������� : escape
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ת�����ݲ����浽 to ������ָ��Ļ�����
// ���ʷ�ʽ : public
// �������� : char * to ����ת������ַ����Ļ�����, ��������СӦ�ñ� (size * 2 + 1) ��
// �������� : const char * from ��Ҫת�����ַ���
// �������� : unsigned long size from �ĳ���
// �� �� ֵ : unsigned long ת�����ֽ���
//*********************************************************
unsigned long sql::mariadb::command::escape(char * to, const char * from, unsigned long size) const noexcept
{
	assert(m_ptr_mysql != nullptr);
	assert(to != nullptr);
	assert(from != nullptr);
	return mysql_real_escape_string(m_ptr_mysql, to, from, size);
}


//*********************************************************
// �������� : escape
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ת�����ݲ����浽�ַ���������
// ���ʷ�ʽ : public
// �������� : const char * data ��Ҫת��������
// �������� : unsigned long size ��Ҫת�������ݵĳ���
// �� �� ֵ : std::string ת������ַ���
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣;
//            ���ִ��ʧ�����׳� std::exception �쳣
//*********************************************************
std::string sql::mariadb::command::escape(const char * data, unsigned long size) const
{
	auto buffer = this->escape_buffer(data, size);
	return std::string(buffer.begin(), buffer.end());
}


//*********************************************************
// �������� : escape_with_quote
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ת�����ݲ��Ե��������ñ��浽�ַ���������
// ���ʷ�ʽ : public
// �������� : const char * data ��Ҫת��������
// �������� : unsigned long size ��Ҫת�������ݵĳ���
// �� �� ֵ : std::string ת������ַ���
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣;
//            ���ִ��ʧ�����׳� std::exception �쳣
//*********************************************************
std::string sql::mariadb::command::escape_with_quote(const char * data, unsigned long size) const
{
	auto buffer = this->escape_buffer_with_quote(data, size);
	return std::string(buffer.begin(), buffer.end());
}


//*********************************************************
// �������� : escape_buffer
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ת�����ݲ����浽 std::vector<char> ���͵Ļ�������
// ���ʷ�ʽ : public
// �������� : const char * data ��Ҫת��������
// �������� : unsigned long size ��Ҫת�������ݵĳ���
// �� �� ֵ : std::vector<char> ����ת��������ݵĻ�����
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣;
//            ���ִ��ʧ�����׳� std::exception �쳣
//*********************************************************
std::vector<char> sql::mariadb::command::escape_buffer(const char * data, unsigned long size) const
{
	assert(m_ptr_mysql != nullptr);
	assert(data != nullptr);

	std::vector<char> buffer(size * 2 + 1);
	auto n = mysql_real_escape_string(m_ptr_mysql, buffer.data(), data, size);
	buffer.resize(n);

	return buffer;
}


//*********************************************************
// �������� : escape_buffer_with_quote
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ת�����ݲ����Ե��������ñ��浽 std::vector<char> ���͵Ļ�������
// ���ʷ�ʽ : public
// �������� : const char * data ��Ҫת��������
// �������� : unsigned long size ��Ҫת�������ݵĳ���
// �� �� ֵ : std::vector<char> ����ת��������ݵĻ�����
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣;
//            ���ִ��ʧ�����׳� std::exception �쳣
//*********************************************************
std::vector<char> sql::mariadb::command::escape_buffer_with_quote(const char * data, unsigned long size) const
{
	assert(m_ptr_mysql != nullptr);
	assert(data != nullptr);

	std::vector<char> buffer(size * 2 + 1 + 2);
	auto n = mysql_real_escape_string(m_ptr_mysql, buffer.data() + 1, data, size);
	buffer.resize(n + 2);
	buffer.front() = '\'';
	buffer.back() = '\'';
	return buffer;
}


//*********************************************************
// �������� : execute
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �������� : const char * text SQL���
// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
//*********************************************************
bool sql::mariadb::command::execute(const char * text) const noexcept
{
	assert(m_ptr_mysql != nullptr);
	return mysql_query(m_ptr_mysql, text) == 0;
}


//*********************************************************
// �������� : execute
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �������� : const char * text SQL���
// �������� : unsigned long length SQL����ַ�������
// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
//*********************************************************
bool sql::mariadb::command::execute(const char * text, unsigned long length) const noexcept
{
	assert(m_ptr_mysql != nullptr);
	return mysql_real_query(m_ptr_mysql, text, length) == 0;
}


//*********************************************************
// �������� : execute
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �������� : const std::string & text SQL���
// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
//*********************************************************
bool sql::mariadb::command::execute(const std::string & text) const noexcept
{
	assert(m_ptr_mysql != nullptr);
	return mysql_real_query(m_ptr_mysql, text.c_str(), text.size()) == 0;
}


//*********************************************************
// �������� : execute
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �������� : const std::vector<char> & data SQL���
// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
//*********************************************************
bool sql::mariadb::command::execute(const std::vector<char>& data) const noexcept
{
	assert(m_ptr_mysql != nullptr);
	return mysql_real_query(m_ptr_mysql, data.data(), data.size()) == 0;
}


//*********************************************************
// �������� : execute
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣;
//            �������SQL���ʧ�����׳� std::exception �쳣
//*********************************************************
bool sql::mariadb::command::execute(void) const
{
	assert(m_ptr_mysql != nullptr);
	assert(!m_text.empty());

	unsigned long size = 0; // �������ݵ����ֽ���, ���ڿ��ٻ�����

	// ��������, ������ݵ����ֽ���
	for (const auto &parameter : m_parameters)
	{
		const auto isnum = parameter.second.first; // �����Ƿ�һ����
		const auto count = parameter.second.second.second; // ���ݴ�С

		// �������һ����, �����������������ŵĳ���
		size += isnum ? count : count + 2;
	}

	// ���ڱ������ɵ�SQL���
	std::vector<char> statement;
	statement.reserve(m_text.size() + (size * 2 + m_parameters.size()));

	// ����ԭSQL����б����� '?' �ĸ���
	unsigned int mark_count = 0;

	// ����SQL���
	for (auto ch : m_text)
	{
		// ����ַ������ʺ�, ��ֱ����Ӹ��ַ��������Ľ�β
		if (ch != '?')
		{
			statement.push_back(ch);
		}
		else // ����ַ����ʺ�
		{
			const auto &parameter = m_parameters.at(mark_count); // ����
			const auto isnum = parameter.first; // �����Ƿ�һ����
			const auto start = parameter.second.first; // ���ݿ�ʼλ��
			const auto length = parameter.second.second; // ���ݴ�С

			// ���������һ����, ��ֱ�Ӹ�������
			if (isnum)
			{
				const auto count = statement.size(); // �����ʱ������С
				statement.resize(count + length); // ��������
				const auto pointer = statement.data() + count; // ���ƵĿ�ʼλ��
				memcpy(pointer, start, length); // ��������
			}
			else // ����������ַ�����������, ��ת�����ݲ���ӵ�����
			{
				statement.push_back('\''); // ��ӵ�����

				const auto count = statement.size(); // �����ʱ������С
				statement.resize(count + (length * 2 + 1)); // ��������
				const auto pointer = statement.data() + count; // ���ƵĿ�ʼλ��
				statement.resize(count + this->escape(pointer, start, length));

				statement.push_back('\''); // ��ӵ�����
			}

			++mark_count; // ����һ���ʺŵĸ���
		}
	}

	// ִ��SQL��䲢�����Ƿ�ִ�гɹ�
	return mysql_real_query(m_ptr_mysql, statement.data(), statement.size()) == 0;
}


//*********************************************************
// �������� : execute_reader
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �������� : const char * text SQL���
// �� �� ֵ : sql::mariadb::recordset �����
// ��    �� : ���ִ��SQLʧ�����׳� mariadb_exception �쳣
//*********************************************************
sql::mariadb::recordset sql::mariadb::command::execute_reader(const char * text) const
{
	if_false_throw(this->execute(text), text);
	return recordset(m_ptr_mysql);
}


//*********************************************************
// �������� : execute_reader
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �������� : const char * text SQL���
// �������� : unsigned long length text���ֽ���
// �� �� ֵ : sql::mariadb::recordset �����
// ��    �� : ���ִ��SQLʧ�����׳� mariadb_exception �쳣
//*********************************************************
sql::mariadb::recordset sql::mariadb::command::execute_reader(const char * text, unsigned long length) const
{
	if_false_throw(this->execute(text, length), std::string(text, length));
	return recordset(m_ptr_mysql);
}


//*********************************************************
// �������� : execute_reader
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �������� : const std::string & text SQL���
// �� �� ֵ : sql::mariadb::recordset �����
// ��    �� : ���ִ��SQLʧ�����׳� mariadb_exception �쳣
//*********************************************************
sql::mariadb::recordset sql::mariadb::command::execute_reader(const std::string & text) const
{
	if_false_throw(this->execute(text), text);
	return recordset(m_ptr_mysql);
}


//*********************************************************
// �������� : execute_reader
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �������� : const std::vector<char> & data SQL���
// �� �� ֵ : sql::mariadb::recordset �����
// ��    �� : ���ִ��SQLʧ�����׳� mariadb_exception �쳣
//*********************************************************
sql::mariadb::recordset sql::mariadb::command::execute_reader(const std::vector<char>& data) const
{
	if_false_throw(this->execute(data), std::string(data.begin(), data.end()));
	return recordset(m_ptr_mysql);
}


//*********************************************************
// �������� : execute_reader
// ��    �� : Gooeen
// ������� : 2015/09/16
// ����˵�� : ִ��SQL���
// ���ʷ�ʽ : public
// �� �� ֵ : sql::mariadb::recordset �����
// ��    �� : ���ִ��SQLʧ�����׳� mariadb_exception �쳣
//            ���������Դʧ�����׳� std::bad_alloc �쳣;
//            �������SQL���ʧ�����׳� std::exception �쳣
//*********************************************************
sql::mariadb::recordset sql::mariadb::command::execute_reader(void) const
{
	if_false_throw(this->execute(), m_text);
	return recordset(m_ptr_mysql);
}
