#include <iostream>
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <vector>
#include <assert.h>

int main(int argc, char** argv)
{
	if ( !boost::filesystem::exists("./rar.exe") )
	{
		std::cout << "找不到RAR.exe压缩程序！";
		return 0;
	}

	boost::format fmtTest("%02d%012d");
	fmtTest % 4 % 6;
	auto strt = fmtTest.str();
	boost::program_options::options_description dscp("description");
	dscp.add_options()
		("srcPath", boost::program_options::value< std::vector<std::string> >(), "srcPath")
		("dstPath", boost::program_options::value< std::string >(), "dstPath");
	boost::program_options::variables_map vmap;

	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, dscp), vmap);
	boost::program_options::notify(vmap);

	if ( 0 == vmap.count("srcPath") )
	{
		std::cout << "备份源路径为空";
		return 0;
	}

	if ( 0 == vmap.count("dstPath") )
	{
		std::cout << "备份目标路径为空";
		return 0;
	}

	auto srcPath = vmap["srcPath"].as< std::vector<std::string> >();
	auto dstPath = vmap["dstPath"].as<std::string>();

	auto now = boost::gregorian::day_clock::local_day();
	auto timeStr = boost::gregorian::to_simple_string(now);

	boost::system::error_code ec;

	boost::filesystem::path fp = dstPath;
	fp /= timeStr;

	if ( !boost::filesystem::exists(fp) )
	{
		boost::filesystem::create_directories(fp, ec);
		if ( ec )
		{
			std::cout << ec.message();
			return 0;
		}
	}

	boost::format fmtDump("svnadmin dump %s --incremental > %s\\%s.dumpfile");
	fmtDump.bind_arg(2, fp.string());

	boost::format fmtRar("rar a -ep %s %s");
	for ( auto& curPath : srcPath )
	{
		boost::filesystem::path pt(curPath);
		
		auto fileName = (fp/(pt.filename().string() + ".dumpfile"));
		auto fileRar = (fp/(pt.filename().string()));

		fmtDump.clear();
		fmtDump % curPath % pt.filename();
		::system(fmtDump.str().c_str());
		

		fmtRar.clear();
		fmtRar % fileRar.string() % fileName.string();
		if ( 0 == ::system(fmtRar.str().c_str()) )
		{
			boost::filesystem::remove(fileName);
		}
	}
}