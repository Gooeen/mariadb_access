//*********************************************************
// CopyRight (C) 2015 Gooeen
// All rights reserved
//
// �ļ����� : mariadb.h
//
// ��ǰ�汾 : 1.0.1
// ��    �� : Gooeen
// ������� : 2015/09/12
//
// ժ    Ҫ : ���ڷ��� MySQL/MariaDB �����
//*********************************************************
#ifndef __SQL_MARIADB_H__
#define __SQL_MARIADB_H__

#include <mysql/mysql.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <tuple>

// ���ָ���ǿ����׳��쳣
#define if_null_throw(pointer, message)\
if (pointer == nullptr)\
{\
	throw mariadb_exception(this->error() + "\r\nSQL: " + message, __FILE__, __LINE__);\
}

// ���Զ�ȡ����, ���û���������׳��쳣
#define read_or_throw(reader, message)\
if (!reader.read())\
{\
	throw mariadb_exception(std::string() + "no data\r\nSQL: " + message, __FILE__, __LINE__);\
}

// ���ݿ����
namespace sql
{
	// MySQL/MariaDB ����
	namespace mariadb
	{
		class connection; // ���ݿ�������
		class command; // ���ݿ�ִ����
		class recordset; // ���ݿ�������

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
		// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
		//*********************************************************
		std::string to_date_string(unsigned int year, unsigned int month, unsigned int day);

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
		// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
		//*********************************************************
		std::string to_time_string(unsigned int hour, unsigned int minute, unsigned int second);

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
		// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
		//*********************************************************
		std::string to_datetime_string(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);

		// MySQL/MariaDB �쳣��
		class mariadb_exception : public std::exception
		{
		public:

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
			//            ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			mariadb_exception(const std::string &text, const std::string &file, int line);

			//*********************************************************
			// �������� : what
			// ��    �� : Gooeen
			// ������� : 2015/09/12
			// ����˵�� : ����쳣��Ϣ
			// ���ʷ�ʽ : public
			// �� �� ֵ : const char * �쳣��Ϣ�ַ���
			//*********************************************************
			const char * what() const override;

		private:
			std::string m_text; // �쳣��Ϣ
		};

		// ���ݿ�������
		// һ�����ݿ�����ֻ������һ���߳�
		class connection
		{
		public:

			//*********************************************************
			// �������� : connection
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ���캯��
			// ���ʷ�ʽ : public
			//*********************************************************
			connection(void) noexcept;

			//*********************************************************
			// �������� : connection
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ת�ƹ��캯��, ת�ƺ�ԭ����Ӧ��ʹ��
			// ���ʷ�ʽ : public
			// �������� : connection && connector ��Ҫת�ƵĶ���
			//*********************************************************
			connection(connection &&connector) noexcept;

			//*********************************************************
			// �������� : ~connection
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ������ݿ�����û�йر���������ʱ�Զ��ر�
			// ���ʷ�ʽ : public
			//*********************************************************
			~connection(void) noexcept;

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
			bool operator!=(std::nullptr_t null) const noexcept;

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
			bool operator==(std::nullptr_t null) const noexcept;

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
			bool open(const char *user, const char *password,
				const char *database, unsigned int port = 3306,
				const char *host = "127.0.0.1",
				const char *unix_socket = nullptr,
				unsigned long flags = 0) const noexcept;

			//*********************************************************
			// �������� : close
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : �ر����ݿ�����, ������ֶ��ر����ݿ�����, ������ʱ�Զ��ر�
			// ���ʷ�ʽ : public
			//*********************************************************
			void close(void) noexcept;

			//*********************************************************
			// �������� : error
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ�����Ϣ
			// ���ʷ�ʽ : public
			// �� �� ֵ : std::string ������Ϣ
			//*********************************************************
			std::string error(void) const noexcept;

			//*********************************************************
			// �������� : errorno
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ������
			// ���ʷ�ʽ : public
			// �� �� ֵ : unsigned int �������
			//*********************************************************
			unsigned int errorno(void) const noexcept;

		private:

			//*********************************************************
			// �������� : connection
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ֹ����
			// ���ʷ�ʽ : private 
			// �������� : const connection &
			//*********************************************************
			connection(const connection &) = delete;

			//*********************************************************
			// �������� : operator=
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ֹ����
			// ���ʷ�ʽ : private 
			// �������� : const connection &
			// �� �� ֵ : connection &
			//*********************************************************
			connection & operator=(const connection &) = delete;

		private:
			friend command;
			friend recordset;
			MYSQL *m_ptr_mysql; // MariaDB ���ݿ���
		};

		// ���ݿ�������
		class recordset
		{
		public:

			//*********************************************************
			// �������� : recordset
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ת�ƹ��캯��, ת�ƺ�ԭ����Ӧ��ʹ��
			// ���ʷ�ʽ : private
			// �������� : recordset && reader ��Ҫת�ƵĶ���
			//*********************************************************
			recordset(recordset &&reader) noexcept;

			//*********************************************************
			// �������� : ~recordset
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��������û���ͷŹر���������ʱ�Զ��ر�
			// ���ʷ�ʽ : public
			//*********************************************************
			~recordset(void) noexcept;

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
			bool operator!=(std::nullptr_t null) const noexcept;

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
			bool operator==(std::nullptr_t null) const noexcept;

			//*********************************************************
			// �������� : close
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : �ͷŽ������ռ�õ���Դ, ������ֶ��ͷ���Դ, ������ʱ�Զ��ͷ�
			// ���ʷ�ʽ : public
			//*********************************************************
			void close(void) noexcept;

			//*********************************************************
			// �������� : error
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ�����Ϣ
			// ���ʷ�ʽ : public
			// �� �� ֵ : std::string ������Ϣ
			//*********************************************************
			std::string error(void) const noexcept;

			//*********************************************************
			// �������� : errorno
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ������
			// ���ʷ�ʽ : public
			// �� �� ֵ : unsigned int �������
			//*********************************************************
			unsigned int errorno(void) const noexcept;

			//*********************************************************
			// �������� : row_count
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ���ݵ�����
			// ���ʷ�ʽ : public
			// �� �� ֵ : unsigned long long ����
			//*********************************************************
			unsigned long long row_count(void) const noexcept;

			//*********************************************************
			// �������� : field_count
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ���ݵ�����(�ֶ�����)
			// ���ʷ�ʽ : public
			// �� �� ֵ : unsigned long ����(�ֶ�����)
			//*********************************************************
			unsigned long field_count(void) const noexcept;

			//*********************************************************
			// �������� : data_size
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ�������ݵĳ���
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��; ����������ݵ���������ܳ�������Խ�������
			// �� �� ֵ : unsigned long �������ݵ��ֽ���
			//*********************************************************
			unsigned long data_size(unsigned long n) const noexcept;

			//*********************************************************
			// �������� : read
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ��һ������
			// ���ʷ�ʽ : public
			// �� �� ֵ : bool �ɹ�����true, ʧ�ܷ���false
			//*********************************************************
			bool read(void) noexcept;

			//*********************************************************
			// �������� : get_char
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����, ��������ǿ��ת���� char ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : char ����
			//*********************************************************
			char get_char(unsigned long n) const;

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
			std::vector<unsigned char> get_udata(unsigned long n) const;

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
			std::vector<char> get_data(unsigned long n) const;

			//*********************************************************
			// �������� : get_double
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����, ��������ת���� double ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : double ����
			//*********************************************************
			double get_double(unsigned long n) const;

			//*********************************************************
			// �������� : get_float
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����, ��������ת���� float ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : float ����
			//*********************************************************
			float get_float(unsigned long n) const;

			//*********************************************************
			// �������� : get_int
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����, ��������ת���� int ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : int ����
			//*********************************************************
			int get_int(unsigned long n) const;

			//*********************************************************
			// �������� : get_long
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����, ��������ת���� long ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : long ����
			//*********************************************************
			long get_long(unsigned long n) const;

			//*********************************************************
			// �������� : get_longlong
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����, ��������ת���� long long ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : long long ����
			//*********************************************************
			long long get_longlong(unsigned long n) const;

			//*********************************************************
			// �������� : get_short
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����, ��������ת���� short ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : short ����
			//*********************************************************
			short get_short(unsigned long n) const;

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
			//            ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			std::string get_string(unsigned long n) const;

			//*********************************************************
			// �������� : get_uchar
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����, ��������ǿ��ת���� unsigned char ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : unsigned char ����
			//*********************************************************
			unsigned char get_uchar(unsigned long n) const;

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
			unsigned int get_uint(unsigned long n) const;

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
			unsigned long get_ulong(unsigned long n) const;

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
			unsigned long long get_ulonglong(unsigned long n) const;

			//*********************************************************
			// �������� : get_ushort
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ��ȡ����, ��������ת���� unsigned short ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : unsigned short ����
			//*********************************************************
			unsigned short get_ushort(unsigned long n) const;

			//*********************************************************
			// �������� : get_wchar
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ��ȡ����, ��������ǿ��ת���� wchar_t ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : wchar_t ����
			//*********************************************************
			wchar_t get_wchar(unsigned long n) const;

			//*********************************************************
			// �������� : get_wstring
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ��ȡ����, ��������ǿ��ת���� std::wstring ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : std::wstring ����
			//*********************************************************
			std::wstring get_wstring(unsigned long n) const;

			//*********************************************************
			// �������� : get_raw
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ������ת��, ֱ�ӻ�ȡ�����ݿⷵ�ص�����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : const char * ����
			//*********************************************************
			const char * get_raw(unsigned long n) const;

			//*********************************************************
			// �������� : get_bool
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ��ȡ����, ��������ǿ��ת���� bool ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : bool ����
			//*********************************************************
			bool get_bool(unsigned long n) const;

			//*********************************************************
			// �������� : get
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��, ��0��ʼ
			// �� �� ֵ : T ����
			//*********************************************************
			template <typename T>
			T get(unsigned long n) const;

			//*********************************************************
			// �������� : get_char
			// ��    �� : Gooeen
			// ������� : 2015/09/13
			// ����˵�� : ��ȡ����, ��������ǿ��ת���� char ����
			// ���ʷ�ʽ : public
			// �������� : unsigned long n ���ݵ���λ��
			// �� �� ֵ : char ����
			//*********************************************************
			template <>
			char get<char>(unsigned long n) const
			{
				return get_char(n);
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
			template <>
			double get<double>(unsigned long n) const
			{
				return get_double(n);
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
			template <>
			float get<float>(unsigned long n) const
			{
				return get_float(n);
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
			template <>
			int get<int>(unsigned long n) const
			{
				return get_int(n);
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
			template <>
			long long get<long long>(unsigned long n) const
			{
				return get_longlong(n);
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
			template <>
			long get<long>(unsigned long n) const
			{
				return get_long(n);
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
			template <>
			short get<short>(unsigned long n) const
			{
				return get_short(n);
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
			//            ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			template <>
			std::string get<std::string>(unsigned long n) const
			{
				return get_string(n);
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
			template <>
			std::vector<unsigned char> get<std::vector<unsigned char>>(unsigned long n) const
			{
				return get_udata(n);
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
			template <>
			std::vector<char> get<std::vector<char>>(unsigned long n) const
			{
				return get_data(n);
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
			template <>
			std::wstring get<std::wstring>(unsigned long n) const
			{
				return get_wstring(n);
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
			template <>
			unsigned char get<unsigned char>(unsigned long n) const
			{
				return get_uchar(n);
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
			template <>
			unsigned int get<unsigned int>(unsigned long n) const
			{
				return get_uint(n);
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
			template <>
			unsigned long long get<unsigned long long>(unsigned long n) const
			{
				return get_ulonglong(n);
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
			template <>
			unsigned long get<unsigned long>(unsigned long n) const
			{
				return get_ulong(n);
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
			template <>
			unsigned short get<unsigned short>(unsigned long n) const
			{
				return get_ushort(n);
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
			template <>
			wchar_t get<wchar_t>(unsigned long n) const
			{
				return get_wchar(n);
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
			template <>
			const char * get<const char *>(unsigned long n) const
			{
				return get_raw(n);
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
			template <>
			bool get<bool>(unsigned long n) const
			{
				return get_bool(n);
			}

		private:

			//*********************************************************
			// �������� : recordset
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : �� command ����󴴽� recordset �����
			// ���ʷ�ʽ : private
			// �������� : MYSQL * pointer ���ݿ���
			//*********************************************************
			recordset(MYSQL *pointer) noexcept;

			//*********************************************************
			// �������� : recordset
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ��ֹ����
			// ���ʷ�ʽ : private
			// �������� : const recordset &
			//*********************************************************
			recordset(const recordset &) = delete;

			//*********************************************************
			// �������� : operator=
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ��ֹ����
			// ���ʷ�ʽ : private
			// �������� : const recordset &
			// �� �� ֵ : recordset &
			//*********************************************************
			recordset & operator=(const recordset &) = delete;

		private:
			friend command;
			MYSQL *m_ptr_mysql; // MariaDB ���ݿ���
			MYSQL_RES *m_ptr_res; // MariaDB ���ݿ�����
			MYSQL_ROW m_row; // MariaDB ���ݿ�����
		};

		// ���ݿ�ִ����
		// һ�� command ����ֻ������һ���߳�
		// ͬһ���߳���, ���Զ�� command ������һ�� ���ݿ����� connection ����
		class command
		{
		public:

			//*********************************************************
			// �������� : command
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ת�ƹ��캯��, ת�ƺ�ԭ����Ӧ��ʹ��
			// ���ʷ�ʽ : public
			// �������� : command && executor ��Ҫת�ƵĶ���
			//*********************************************************
			command(command &&executor) noexcept;

			//*********************************************************
			// �������� : command
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ���캯��
			// ���ʷ�ʽ : public
			// �������� : const connection & connector ���ݿ�������
			//*********************************************************
			explicit command(const connection &connector) noexcept;

			//*********************************************************
			// �������� : command
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ���캯��
			// ���ʷ�ʽ : public
			// �������� : const connection & connector ���ݿ�������
			// �������� : const std::string & text SQL���
			// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			command(const connection &connector, const std::string &text);

			//*********************************************************
			// �������� : command
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ���캯��
			// ���ʷ�ʽ : public
			// �������� : const connection & connector ���ݿ�������
			// �������� : std::string && text SQL���
			//*********************************************************
			command(const connection &connector, std::string &&text) noexcept;

			//*********************************************************
			// �������� : error
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ�����Ϣ
			// ���ʷ�ʽ : public 
			// �������� : void
			// �� �� ֵ : std::string ������Ϣ
			//*********************************************************
			std::string error(void) const noexcept;

			//*********************************************************
			// �������� : errorno
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : ��ȡ���ݿ����ʧ�ܵĴ������
			// ���ʷ�ʽ : public
			// �� �� ֵ : unsigned int �������
			//*********************************************************
			unsigned int errorno(void) const noexcept;

			//*********************************************************
			// �������� : prepare
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : Ԥ��SQL���; ��ʹ���������ӷ�ע��
			//            executer.prepare("select * from table where id = ?");
			//            executer.add(12);
			// ���ʷ�ʽ : public
			// �������� : std::string && text SQL���
			//*********************************************************
			void prepare(std::string &&text) noexcept;

			//*********************************************************
			// �������� : prepare
			// ��    �� : Gooeen
			// ������� : 2015/09/15
			// ����˵�� : Ԥ��SQL���; ��ʹ���������ӷ�ע��
			//            executer.prepare("select * from table1 where id = ?");
			//            executer.add(0, 12);
			// ���ʷ�ʽ : public
			// �������� : const std::string & text SQL���
			// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			void prepare(const std::string &text);

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
			// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			void add(unsigned int pos, const std::string &text);

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
			// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			void add(unsigned int pos, std::string &&text);

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
			// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			void add(unsigned int pos, const std::vector<char> &data);

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
			// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			void add(unsigned int pos, std::vector<char> &&data);

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
			// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			void add(unsigned int pos, const std::vector<unsigned char> &data);

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
			// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			void add(unsigned int pos, std::vector<unsigned char> &&data);

			//*********************************************************
			// �������� : add
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : �������, ִ��SQL���ʱ��������ӵ�����
			//            ����SQL����е� pos ���ʺ�; ��������:
			//            executer.prepare("insert into table1 values(?, ?)");
			//            executer.add(0, 12);
			//            executer.add(1, "data");
			//            ��ִ��SQL���ʱ������ SQL���:
			//            insert into table1 values(12, 'data')
			// ���ʷ�ʽ : public
			// �������� : unsigned int pos ��Ҫ�����ʺŵ�λ��, ��0��ʼ
			// �������� : const std::string & text ����, ��������SQL����н��Ե���������
			// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣
			//*********************************************************
			template <typename T>
			void add(unsigned int pos, T value)
			{
				// �����ַ���
				m_strings.push_back(std::to_string(value));

				// �������ݵ���Ϣ
				const auto p = m_strings.back().c_str();
				const auto size = m_strings.back().size();
				const auto pair = std::make_pair(p, size);
				m_parameters[pos] = std::make_pair(true, pair);
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
			unsigned long escape(char *to, const char *from, unsigned long size) const noexcept;

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
			std::string escape(const char *data, unsigned long size) const;

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
			std::string escape_with_quote(const char *data, unsigned long size) const;

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
			std::vector<char> escape_buffer(const char *data, unsigned long size) const;

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
			std::vector<char> escape_buffer_with_quote(const char *data, unsigned long size) const;

			//*********************************************************
			// �������� : execute
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL���
			// ���ʷ�ʽ : public
			// �������� : const char * text SQL���
			// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
			//*********************************************************
			bool execute(const char *text) const noexcept;

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
			bool execute(const char *text, unsigned long length) const noexcept;

			//*********************************************************
			// �������� : execute
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL���
			// ���ʷ�ʽ : public
			// �������� : const std::string & text SQL���
			// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
			//*********************************************************
			bool execute(const std::string &text) const noexcept;

			//*********************************************************
			// �������� : execute
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL���
			// ���ʷ�ʽ : public
			// �������� : const std::vector<char> & data SQL���
			// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
			//*********************************************************
			bool execute(const std::vector<char> &data) const noexcept;

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
			bool execute(void) const;

			//*********************************************************
			// �������� : execute
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL���; ʹ������:
			//            command(connector).execute("insert into table1 values(?, ?)", std::make_tuple(12, "abc"));
			//            ִ��������佫����SQL���: insert into table1 values(12, 'abc')
			//            ����SQL����ִ��SQL���
			// ���ʷ�ʽ : public
			// �������� : const char * text SQL���
			// �������� : const Tuple & t ����
			// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			bool execute(const std::string &text, const Tuple &t) const
			{
				return this->execute(statement<Tuple>().generate(*this, text, t));
			}

			//*********************************************************
			// �������� : execute
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL���
			// ���ʷ�ʽ : public 
			// �������� : const std::vector<char> & data SQL���
			// �������� : const Tuple & t ����
			// �� �� ֵ : bool ���SQL���ִ�гɹ�����true; ��֮����false
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			bool execute(const std::vector<char> &data, const Tuple &t) const
			{
				return this->execute(statement<Tuple>().generate(*this, data, t));
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
			recordset execute_reader(const char *text) const;

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
			recordset execute_reader(const char *text, unsigned long length) const;

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
			recordset execute_reader(const std::string &text) const;

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
			recordset execute_reader(const std::vector<char> &data) const;

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
			recordset execute_reader(void) const;

			//*********************************************************
			// �������� : execute_reader
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL���; ʹ������:
			//            command(connector).execute_reader("insert into table1 values(?, ?)", std::make_tuple(12, "abc"));
			//            ִ��������佫����SQL���: insert into table1 values(12, 'abc')
			//            ����SQL����ִ��SQL���
			// ���ʷ�ʽ : public
			// �������� : const std::string & text SQL���
			// �������� : const Tuple & t ����
			// �� �� ֵ : sql::mariadb::recordset �����
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			recordset execute_reader(const std::string &text, const Tuple &t) const
			{
				return this->execute_reader(statement<Tuple>().generate(*this, text, t));
			}

			//*********************************************************
			// �������� : execute_reader
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL���
			// ���ʷ�ʽ : public 
			// �������� : const std::vector<char> & data SQL���
			// �������� : const Tuple & t ����
			// �� �� ֵ : sql::mariadb::recordset �����
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			recordset execute_reader(const std::vector<char> &data, const Tuple &t) const
			{
				return this->execute_reader(statement<Tuple>().generate(*this, data, t));
			}

			//*********************************************************
			// �������� : execute_scalar
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ�еĵ�һ��ֵ
			// ���ʷ�ʽ : public
			// �������� : const char * text SQL���
			// �� �� ֵ : T ������е�һ�еĵ�һ��ֵ
			// ��    �� : ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename T>
			T execute_scalar(const char *text) const
			{
				auto reader = this->execute_reader(text);
				if_null_throw(reader, text);
				read_or_throw(reader, text);
				return reader.get<T>(0);
			}

			//*********************************************************
			// �������� : execute_scalar
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ�еĵ�һ��ֵ
			// ���ʷ�ʽ : public
			// �������� : const char * text SQL���
			// �������� : unsigned long length text���ֽ���
			// �� �� ֵ : T ������е�һ�еĵ�һ��ֵ
			// ��    �� : ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename T>
			T execute_scalar(const char *text, unsigned long length) const
			{
				auto reader = this->execute_reader(text, length);
				if_null_throw(reader, std::string(text, length));
				read_or_throw(reader, std::string(text, length));
				return reader.get<T>(0);
			}

			//*********************************************************
			// �������� : execute_scalar
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ�еĵ�һ��ֵ
			// ���ʷ�ʽ : public
			// �������� : const std::string & text SQL���
			// �� �� ֵ : T ������е�һ�еĵ�һ��ֵ
			// ��    �� : ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename T>
			T execute_scalar(const std::string &text) const
			{
				auto reader = this->execute_reader(text);
				if_null_throw(reader, text);
				read_or_throw(reader, text);
				return reader.get<T>(0);
			}

			//*********************************************************
			// �������� : execute_scalar
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ�еĵ�һ��ֵ
			// ���ʷ�ʽ : public
			// �������� : const std::vector<char> & data SQL���
			// �� �� ֵ : T ������е�һ�еĵ�һ��ֵ
			// ��    �� : ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename T>
			T execute_scalar(const std::vector<char> &data) const
			{
				auto reader = this->execute_reader(data);
				if_null_throw(reader, std::string(data.begin(), data.end()));
				read_or_throw(reader, std::string(data.begin(), data.end()));
				return reader.get<T>(0);
			}

			//*********************************************************
			// �������� : execute_scalar
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ�еĵ�һ��ֵ
			// ���ʷ�ʽ : public
			// �� �� ֵ : T ������е�һ�еĵ�һ��ֵ
			// ��    �� : ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename T>
			T execute_scalar(void) const
			{
				auto reader = this->execute_reader();
				if_null_throw(reader, m_text);
				read_or_throw(reader, m_text);
				return reader.get<T>(0);
			}

			//*********************************************************
			// �������� : execute_scalar
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ�еĵ�һ��ֵ
			// ���ʷ�ʽ : public
			// �������� : const std::string & text SQL���
			// �������� : const Tuple & t ����
			// �� �� ֵ : T ������е�һ�еĵ�һ��ֵ
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename T, typename Tuple>
			T execute_scalar(const std::string &text, const Tuple &t) const
			{
				return this->execute_scalar<T>(statement<Tuple>().generate(*this, text, t));
			}

			//*********************************************************
			// �������� : execute_scalar
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ�еĵ�һ��ֵ
			// ���ʷ�ʽ : public
			// �������� : const std::vector<char> & data SQL���
			// �������� : const Tuple & t ����
			// �� �� ֵ : T ������е�һ�еĵ�һ��ֵ
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename T, typename Tuple>
			T execute_scalar(const std::vector<char> &data, const Tuple &t) const
			{
				return this->execute_scalar<T>(statement<Tuple>().generate(*this, data, t));
			}

			//*********************************************************
			// �������� : query
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ������
			// ���ʷ�ʽ : public
			// �������� : const char * text SQL���
			// �� �� ֵ : Tuple ������е�һ������
			// ��    �� : ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename Tuple>
			Tuple query(const char *text) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(text);
				if_null_throw(reader, text);
				read_or_throw(reader, text);
				return data_tuple<Tuple>::get(reader);
			}

			//*********************************************************
			// �������� : query
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ������
			// ���ʷ�ʽ : public
			// �������� : const char * text SQL���
			// �������� : unsigned long length text���ֽ���
			// �� �� ֵ : Tuple ������е�һ������
			// ��    �� : ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename Tuple>
			Tuple query(const char *text, unsigned long length) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(text, length);
				if_null_throw(reader, std::string(text, length));
				read_or_throw(reader, std::string(text, length));
				return data_tuple<Tuple>::get(reader);
			}

			//*********************************************************
			// �������� : query
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ������
			// ���ʷ�ʽ : public
			// �������� : const std::string & text SQL���
			// �� �� ֵ : Tuple ������е�һ������
			// ��    �� : ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename Tuple>
			Tuple query(const std::string &text) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(text);
				if_null_throw(reader, text);
				read_or_throw(reader, text);
				return data_tuple<Tuple>::get(reader);
			}

			//*********************************************************
			// �������� : query
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ������
			// ���ʷ�ʽ : public 
			// �������� : const std::vector<char> & data SQL���
			// �� �� ֵ : Tuple ������е�һ������
			// ��    �� : ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename Tuple>
			Tuple query(const std::vector<char> &data) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(data);
				if_null_throw(reader, std::string(data.begin(), data.end()));
				read_or_throw(reader, std::string(data.begin(), data.end()));
				return data_tuple<Tuple>::get(reader);
			}

			//*********************************************************
			// �������� : query
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ������
			// ���ʷ�ʽ : public
			// �� �� ֵ : Tuple ������е�һ������
			// ��    �� :  ���ִ��SQLʧ�ܻ��߲��ܻ�ȡ���������ת��ʧ�����׳� mariadb_exception �쳣
			//*********************************************************
			template <typename Tuple>
			Tuple query(void) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader();
				if_null_throw(reader, m_text);
				read_or_throw(reader, m_text);
				return data_tuple<Tuple>::get(reader);
			}

			//*********************************************************
			// �������� : query
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ������
			// ���ʷ�ʽ : public
			// �������� : const std::string & text SQL���
			// �������� : const InTuple & t ����
			// �� �� ֵ : OutTuple ������е�һ������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename OutTuple, typename InTuple>
			OutTuple query(const std::string &text, const InTuple &t) const
			{
				return this->query<OutTuple>(statement<InTuple>().generate(*this, text, t));
			}

			//*********************************************************
			// �������� : query
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢���ص�һ������
			// ���ʷ�ʽ : public
			// �������� : const std::vector<char> & data SQL���
			// �������� : const InTuple & t ����
			// �� �� ֵ : OutTuple ������е�һ������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename OutTuple, typename InTuple>
			OutTuple query(const std::vector<char> &data, const InTuple &t) const
			{
				return this->query<OutTuple>(statement<InTuple>().generate(*this, data, t));
			}

			//*********************************************************
			// �������� : query_vector
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �������� : const char * text SQL���
			// �� �� ֵ : std::vector<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::vector<Tuple> query_vector(const char *text) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(text);
				if_null_throw(reader, text);
				return vector_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_vector
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public 
			// �������� : const char * text SQL���
			// �������� : unsigned long length text���ֽ���
			// �� �� ֵ : std::vector<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::vector<Tuple> query_vector(const char *text, unsigned long length) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(text, length);
				if_null_throw(reader, std::string(text, length));
				return vector_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_vector
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public 
			// �������� : const std::string & text SQL���
			// �� �� ֵ : std::vector<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::vector<Tuple> query_vector(const std::string &text) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(text);
				if_null_throw(reader, text);
				return vector_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_vector
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �������� : const std::vector<char> & data SQL���
			// �� �� ֵ : std::vector<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::vector<Tuple> query_vector(const std::vector<char> &data) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(data);
				if_null_throw(reader, std::string(data.begin(), data.end()));
				return vector_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_vector
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �� �� ֵ : std::vector<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::vector<Tuple> query_vector(void) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader();
				if_null_throw(reader, m_text);
				return vector_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_vector
			// ��    �� : Gooeen
			// ������� : 2015/09/17
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �������� : const std::string & text SQL���
			// �������� : const InTuple & t ����
			// �� �� ֵ : std::vector<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename OutTuple, typename InTuple>
			std::vector<OutTuple> query_vector(const std::string &text, const InTuple &t) const
			{
				return this->query_vector<OutTuple>(statement<InTuple>().generate(*this, text, t));
			}

			//*********************************************************
			// �������� : query_vector
			// ��    �� : Gooeen
			// ������� : 2015/09/17
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �������� : const std::vector<char> & data SQL���
			// �������� : const InTuple & t ����
			// �� �� ֵ : std::vector<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename OutTuple, typename InTuple>
			std::vector<OutTuple> query_vector(const std::vector<char> &data, const InTuple &t) const
			{
				return this->query_vector<OutTuple>(statement<InTuple>().generate(*this, data, t));
			}

			//*********************************************************
			// �������� : query_list
			// ��    �� : Gooeen
			// ������� : 2015/09/17
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �������� : const char * text SQL���
			// �� �� ֵ : std::list<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::list<Tuple> query_list(const char *text) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(text);
				if_null_throw(reader, text);
				return list_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_list
			// ��    �� : Gooeen
			// ������� : 2015/09/17
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �������� : const char * text SQL���
			// �������� : unsigned long length text���ֽ���
			// �� �� ֵ : std::list<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::list<Tuple> query_list(const char *text, unsigned long length) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(text, length);
				if_null_throw(reader, std::string(text, length));
				return list_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_list
			// ��    �� : Gooeen
			// ������� : 2015/09/17
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �������� : const std::string & text SQL���
			// �� �� ֵ : std::list<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::list<Tuple> query_list(const std::string &text) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(text);
				if_null_throw(reader, text);
				return list_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_list
			// ��    �� : Gooeen
			// ������� : 2015/09/17
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �������� : const std::vector<char> & data SQL���
			// �� �� ֵ : std::list<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::list<Tuple> query_list(const std::vector<char> &data) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader(data);
				if_null_throw(reader, std::string(data.begin(), data.end()));
				return list_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_list
			// ��    �� : Gooeen
			// ������� : 2015/09/17
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public
			// �� �� ֵ : std::list<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			std::list<Tuple> query_list(void) const
			{
				// ��ȡ�����, �����ȡʧ�����׳��쳣
				auto reader = this->execute_reader();
				if_null_throw(reader, m_text);
				return list_from_recordset<Tuple>(reader);
			}

			//*********************************************************
			// �������� : query_list
			// ��    �� : Gooeen
			// ������� : 2015/09/17
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public 
			// �������� : const std::string & text SQL���
			// �������� : const InTuple & t ����
			// �� �� ֵ : std::list<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename OutTuple, typename InTuple>
			std::list<OutTuple> query_list(const std::string &text, const InTuple &t) const
			{
				return this->query_list<OutTuple>(statement<InTuple>().generate(*this, text, t));
			}

			//*********************************************************
			// �������� : query_list
			// ��    �� : Gooeen
			// ������� : 2015/09/17
			// ����˵�� : ִ��SQL��䲢������������
			// ���ʷ�ʽ : public 
			// �������� : const std::vector<char> & data SQL���
			// �������� : const InTuple & t ����
			// �� �� ֵ : std::list<Tuple> ���������������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename OutTuple, typename InTuple>
			std::list<OutTuple> query_list(const std::vector<char> &data, const InTuple &t) const
			{
				return this->query_list<OutTuple>(statement<InTuple>().generate(*this, data, t));
			}

		private:

			//*********************************************************
			// �������� : vector_from_recordset
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : �ӽ�����ж�ȡ���ݲ���䵽 std::vector �����з���
			// ���ʷ�ʽ : private
			// �������� : recordset & reader �����
			// �� �� ֵ : std::vector<Tuple> ������е���������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			static std::vector<Tuple> vector_from_recordset(recordset &reader)
			{
				// �����ڴ�
				std::vector<Tuple> data; // ��������
				data.reserve((unsigned int)reader.row_count());

				// ��ֵ
				while (reader.read())
				{
					data.push_back(data_tuple<Tuple>::get(reader));
				}
				return data;
			}

			//*********************************************************
			// �������� : list_from_recordset
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : �ӽ�����ж�ȡ���ݲ���䵽 std::list �����з���
			// ���ʷ�ʽ : private
			// �������� : recordset & reader �����
			// �� �� ֵ : std::list<Tuple> ������е���������
			// ��    �� : ���ִ��ʧ�����׳��쳣
			//*********************************************************
			template <typename Tuple>
			static std::list<Tuple> list_from_recordset(recordset &reader)
			{
				// ��ֵ
				std::list<Tuple> data; // ��������
				while (reader.read())
				{
					data.push_back(data_tuple<Tuple>::get(reader));
				}
				return data;
			}

			// ���ڽ�������ж�ȡ�������ݱ��浽 tuple �����в�����
			template <typename Tuple>
			struct data_tuple
			{
				//*********************************************************
				// �������� : get
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : ���شӽ�����ж�ȡ��������, ���� tuple ���󱣴�
				// ���ʷ�ʽ : public
				// �������� : const recordset & reader ���������
				// �� �� ֵ : Tuple �ӽ�����ж�ȡ��������
				// ��    �� : ���ִ��ʧ�����׳��쳣
				//*********************************************************
				static Tuple get(const recordset &reader)
				{
					Tuple t;
					set<std::tuple_size<Tuple>::value>(t, reader);
					return t;
				}

				//*********************************************************
				// �������� : set
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : ��������ж�ȡ�������ݱ��浽ָ���Ĳ��� t ��
				// ģ����� : size_t Size �������� t ��Ԫ������
				// ���ʷ�ʽ : public
				// �������� : Tuple & t ���ڽ��ӽ�����ж�ȡ����������䵽�ò���ָ��λ��
				// �������� : const recordset & reader ���������
				// ��    �� : ���ִ��ʧ�����׳��쳣
				//*********************************************************
				template <size_t Size>
				static void set(Tuple &t, const recordset &reader)
				{
					const auto n = Size - 1;
					std::get<n>(t) = reader.get<std::tuple_element<n, Tuple>::type>(n);
					set<n>(t, reader);
				}

				//*********************************************************
				// �������� : set
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : ��������ж�ȡ�������ݱ��浽ָ���Ĳ��� t ��
				// ģ����� : size_t Size �������� t ��Ԫ������
				// ���ʷ�ʽ : public
				// �������� : Tuple & t ���ڽ��ӽ�����ж�ȡ����������䵽�ò���ָ��λ��
				// �������� : const recordset & reader ���������
				// ��    �� : ���ִ��ʧ�����׳��쳣
				//*********************************************************
				template <>
				static void set<1>(Tuple &t, const recordset &reader)
				{
					std::get<0>(t) = reader.get<std::tuple_element<0, Tuple>::type>(0);
				}
			};

			// �����ṩ�Ĵ��ʺŵ�SQL���� tuple ��������������SQL���
			// tuple ��Ԫ������ֻ���� �� �� std::string ���ַ���
			// �� std::vector<char> �ͺ� std::vector<unsigned char> �ͻ�����
			template <typename Tuple>
			class statement
			{
			private:

				//*********************************************************
				// �������� : escape_buffer
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : ����ת�����ַ��������浽 std::vector<char> ������
				// ���ʷ�ʽ : public
				// �������� : const command & command ����
				// �������� : Type type ��
				// �� �� ֵ : std::vector<char> �ַ�����ʽ������
				// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣;
				//            ���ִ��ʧ�����׳� std::exception �쳣
				//*********************************************************
				template <typename Type>
				static std::vector<char> escape_buffer(const command &, Type type)
				{
					const auto text = std::to_string(type);
					return std::vector<char>(text.begin(), text.end());
				}

				//*********************************************************
				// �������� : escape_buffer
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : ת������
				// ���ʷ�ʽ : public
				// �������� : const command & executer command ����
				// �������� : const std::string & data �ַ���
				// �� �� ֵ : std::vector<char> �ַ�����ʽ������
				// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣;
				//            ���ִ��ʧ�����׳� std::exception �쳣
				//*********************************************************
				static std::vector<char> escape_buffer(const command &executer, const std::string &data)
				{
					return executer.escape_buffer_with_quote(data.c_str(), data.size());
				}

				//*********************************************************
				// �������� : escape_buffer
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : ת������
				// ���ʷ�ʽ : public
				// �������� : const command & executer command ����
				// �������� : const std::vector<char> & data ����
				// �� �� ֵ : std::vector<char> �ַ�����ʽ������
				// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣;
				//            ���ִ��ʧ�����׳� std::exception �쳣
				//*********************************************************
				static std::vector<char> escape_buffer(const command &executer, const std::vector<char> &data)
				{
					return executer.escape_buffer_with_quote(data.data(), data.size());
				}

				//*********************************************************
				// �������� : escape_buffer
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : ת������
				// ���ʷ�ʽ : public
				// �������� : const command & executer command ����
				// �������� : const std::vector<unsigned char> & data ����
				// �� �� ֵ : std::vector<char> �ַ�����ʽ������
				// ��    �� : ���������Դʧ�����׳� std::bad_alloc �쳣;
				//            ���ִ��ʧ�����׳� std::exception �쳣
				//*********************************************************
				static std::vector<char> escape_buffer(const command &executer, const std::vector<unsigned char> &data)
				{
					return executer.escape_buffer_with_quote((const char *)data.data(), data.size());
				}

				//*********************************************************
				// �������� : escape
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : �� t ����������ݽ���ת���󱣴浽 list ������
				// ���ʷ�ʽ : public
				// �������� : const command & executer command ����
				// �������� : const Tuple & t ����
				// �� �� ֵ : std::list<std::vector<char>> ת���������
				// ��    �� : ���ִ��ʧ�����׳� std::exception �쳣
				//*********************************************************
				template <size_t size>
				std::list<std::vector<char>> escape(const command &executer, const Tuple &t)
				{
					// ��������
					std::list<std::vector<char>> datas = escape<size - 1>(executer, t);
					datas.push_back(escape_buffer(executer, std::get<size - 1>(t)));

					// ����ֽ���
					m_size += datas.back().size();

					return datas;
				}

				//*********************************************************
				// �������� : escape
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : �� t ����������ݽ���ת���󱣴浽 list ������
				// ���ʷ�ʽ : public
				// �������� : const command & executer command ����
				// �������� : const Tuple & t ����
				// �� �� ֵ : std::list<std::vector<char>> ת���������
				// ��    �� : ���ִ��ʧ�����׳� std::exception �쳣
				//*********************************************************
				template <>
				std::list<std::vector<char>> escape<1>(const command &executer, const Tuple &t)
				{
					// ��������
					std::list<std::vector<char>> datas;
					datas.push_back(escape_buffer(executer, std::get<0>(t)));

					// ����ֽ���
					m_size += datas.back().size();

					return datas;
				}

			public:

				//*********************************************************
				// �������� : generate
				// ��    �� : Gooeen
				// ������� : 2015/09/16
				// ����˵�� : ����SQL��䲢���浽 std::vector<char> ������
				// ���ʷ�ʽ : public
				// �������� : const command & executer command ����
				// �������� : const Type & text SQL���, std::string ���� std::vector<char>
				// �������� : const Tuple & t ����
				// �� �� ֵ : std::vector<char> ������SQL���
				// ��    �� : ���ִ��ʧ�����׳� std::exception �쳣
				//*********************************************************
				template <typename Type>
				std::vector<char> generate(const command &executer, const Type &text, const Tuple &t)
				{
					// ��������������SQL�����ֽ���
					m_size = text.size();

					// ��ȥ�ʺ�����
					m_size -= std::count(text.begin(), text.end(), '?');

					// ��ȡת��������ݲ��Ҽ�������������SQL�����ֽ���
					std::list<std::vector<char>> datas = this->escape<std::tuple_size<Tuple>::value>(executer, t);

					// ���ڱ�������������SQL���
					std::vector<char> data(m_size);

					// ���ڸ�ֵ����
					auto vect_iter = data.begin();
					auto list_iter = datas.begin();

					// ����SQL���
					for (auto ch : text)
					{
						// ����������ʺ���ֱ�Ӹ����ַ�
						if (ch != '?')
						{
							*vect_iter = ch;
							++vect_iter;
						}
						// ��������ʺ���������
						else
						{
							const auto p = data.data() + (vect_iter - data.begin());
							memcpy(p, list_iter->data(), list_iter->size());
							vect_iter += list_iter->size();
							++list_iter;
						}
					}

					return data;
				}

			private:
				size_t m_size; // ����������SQL�����ֽ���
			};

		private:

			//*********************************************************
			// �������� : command
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ��ֹ����
			// ���ʷ�ʽ : private
			// �������� : const command &
			//*********************************************************
			command(const command &) = delete;

			//*********************************************************
			// �������� : operator=
			// ��    �� : Gooeen
			// ������� : 2015/09/16
			// ����˵�� : ��ֹ����
			// ���ʷ�ʽ : private
			// �������� : const command &
			// �� �� ֵ : command &
			//*********************************************************
			command & operator=(const command &) = delete;

		private:
			friend recordset;
			typedef std::pair<const char *, unsigned long> byte_data; // ���ݿ�ʼλ�úʹ�С
			typedef std::pair<bool, byte_data> alnum_data; // �Ƿ�һ����������

			MYSQL *m_ptr_mysql; // MariaDB ���ݿ���
			std::string m_text; // SQL���
			std::list<std::string> m_strings; // �����ַ�������
			std::list<std::vector<char>> m_datas; // ��������
			std::list<std::vector<unsigned char>> m_udatas; // ��������
			std::map<unsigned int, alnum_data> m_parameters; // ����SQL��������
		};
	}
}

#undef if_null_throw
#undef read_or_throw

#endif // __SQL_MARIADB_H__

