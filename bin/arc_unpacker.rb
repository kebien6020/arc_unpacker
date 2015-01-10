#!/usr/bin/ruby -W2
require_relative '../lib/common'
require 'fileutils'
require 'lib/cli_archive'
require 'lib/binary_io'
require 'lib/output_files'
require 'lib/factory/archive_factory'

# CLI frontend
class ArchiveUnpacker < CLIArchive
  def run_internal
    verbose = @options[:verbosity] != :quiet

    unless @options[:archive].nil?
      unpack(@options[:archive], @options)
      return
    end

    ArchiveFactory.each do |key, archive|
      @options[:format] = key
      @options[:archive] = archive
      print format('Trying %s... ', key) if verbose
      begin
        unpack(archive, @options)
        puts 'ok' if verbose
        return
      rescue => e
        puts e.message if verbose
        if @options[:verbosity] == :debug && !e.is_a?(RecognitionError)
          puts
          puts e.backtrace
          puts
        end
        next
      end
    end
  end

  def unpack(archive, options)
    fail 'Unpacking not supported' unless defined? archive::Unpacker
    archive.parse_cli_options(@arg_parser, options)

    FileUtils.mkpath(options[:output_path])
    output_files = OutputFiles.new(options[:output_path], options)
    unpacker = archive::Unpacker.new
    BinaryIO.from_file(options[:input_path], 'rb') do |arc_file|
      unpacker.unpack(arc_file, output_files, options)
    end
  end
end

ArchiveUnpacker.new.run
