#ifndef BUNSAN_PM_CONFIG_HPP
#define BUNSAN_PM_CONFIG_HPP

namespace bunsan{namespace pm{namespace config
{
	namespace dir
	{
		constexpr const char *tmp = "dir.tmp";		///< directory for large temporary files
		constexpr const char *package = "dir.package";	///< directory for built packages
		constexpr const char *source = "dir.source";	///< directory for package sources
	}
	/// file and directory names, usually relative
	namespace name
	{
		namespace file
		{
			constexpr const char *tmp = "name.file.tmp";					///< file mask for small temporary files, should be absolute
			constexpr const char *index = "name.file.index";				///< index file name
			constexpr const char *checksum = "name.file.checksum";				///< checksum file name
			constexpr const char *build_snapshot = "name.file.build_snapshot";		///< build snapshot file name
			constexpr const char *installation_snapshot = "name.file.installation_snapshot";///< installation snapshot file name
			constexpr const char *build = "name.file.build";				///< build archive name
			constexpr const char *installation = "name.file.installation";			///< installation archive name
		}
		namespace dir
		{
			constexpr const char *source = "name.dir.source";		///< subdirectory for package source
			constexpr const char *build = "name.dir.build";			///< subdirectory for package building
			constexpr const char *installation = "name.dir.installation";	///< subdirectory for package installation
		}
	}
	namespace suffix
	{
		constexpr const char *archive = "suffix.archive";	///< suffix of archive files
	}
	namespace command
	{
		constexpr const char *fetch = "command.fetch";		///< download command: fetch(source, destination)
		constexpr const char *unpack = "command.unpack";	///< unpack command: unpack(source, destination)
		constexpr const char *pack = "command.pack";		///< pack command, \todo encapsulation fault documentation
		constexpr const char *configure = "command.configure";	///< configure command
		constexpr const char *compile = "command.compile";	///< compilation command
		constexpr const char *install = "command.install";	///< install command
	}
	constexpr const char *repository_url = "repository_url";	///< repository location
}}}

#endif //BUNSAN_PM_CONFIG_HPP

