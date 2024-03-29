#include <asl/Path.h>
#include <asl/Directory.h>

namespace asl {

#ifdef _WIN32
#define SEP '\\'
#else
#define SEP '/'
#endif

String Path::name() const
{
	int n = _path.lastIndexOf(SEP);
#ifdef _WIN32
	int m = _path.lastIndexOf('/');
	n = (m > n)? m : n;
#endif
	return _path.substring(n+1);
}

String Path::extension() const
{
	int n = _path.lastIndexOf(SEP);
#ifdef _WIN32
	int m = _path.lastIndexOf('/');
	n = (m > n) ? m : n;
#endif
	int dot = _path.lastIndexOf('.');
	return (dot >= 0 && dot > n) ? _path.substring(dot + 1) : String();
}

bool Path::hasExtension(const String& extensions) const
{
	String ext = extension().toLowerCase();
	if (!extensions.contains('|'))
		return extensions.toLowerCase() == ext;
	Array<String> exts = extensions.toLowerCase().split('|');
	for (int i = 0; i < exts.length(); i++)
		if (ext == exts[i])
			return true;
	return false;
}

Path Path::directory() const
{
	int n = _path.lastIndexOf(SEP);
#ifdef _WIN32
	int m = _path.lastIndexOf('/');
	n = (m > n)? m : n;
#endif
	if (n < 0 && _path.contains(':')) // drive
		return "";
	return (n>=0)? _path.substring(0, n) : String(".");
}

Path& Path::removeDDots()
{
	Array<String> parts = _path.split('/');
	bool unc = _path.startsWith("//");
	for (int i = 1; i < parts.length(); i++)
		if (parts[i] == "" || parts[i] == ".")
			parts.remove(i--);

	for(int i=1; i<parts.length(); i++)
	{
		if(parts[i]=="..") {
			parts.remove(i > 1? i-1 : i, i > 1 ? 2 : 1);
			i-=2;
		}
	}
	_path = parts.join('/');

	if (unc)
		_path = "/" + _path;

	return *this;
}

Path Path::absolute() const
{
	if (isAbsolute()) {
		Path p(_path);
		p.removeDDots();
		return p;
	}
	String current = Directory::current();
	current.replaceme('\\', '/');
	String all;
	all << current << '/' << (_path.startsWith("./")? _path.substring(2) : _path);
	Path p(all);
	p.removeDDots();
	return p;
}

bool Path::isAbsolute() const
{
	return _path[0] == '/' || _path[0] == '\\' || (_path.length() > 1 && _path[1] == ':');
}

Path Path::noExt() const
{
	int n = _path.lastIndexOf(SEP);
#ifdef _WIN32
	int m = _path.lastIndexOf('/');
	n = (m > n) ? m : n;
#endif
	int dot = _path.lastIndexOf('.');
	return (dot >= 0 && dot > n) ? _path.substring(0, dot) : _path;
}

}
