##
# Contains all logic for gathering and displaying backtraces.

class Rubinius::Backtrace
  include Enumerable

  MAX_WIDTH_PERCENTAGE = 45
  MIN_WIDTH = 20

  # If passed nil, we assume someone forgot to create a backtrace
  # in the VM.
  def self.backtrace(locations)
    locations ? new(locations) : new([Rubinius::Location::Missing.new])
  end

  def initialize(locations, width = Rubinius::TERMINAL_WIDTH, min_width = MIN_WIDTH)
    color_config = Rubinius::Config["rbx.colorize_backtraces"]
    if ENV['RBX_NOCOLOR'] or color_config == "no" or color_config == "NO"
      @colorize = false
    else
      @colorize = true
    end

    @locations = locations
    @bold = "\033[0;1m"
    @inline_effect = "\033[0;4m"

    @width = width
    @min_width = min_width

    @mri_backtrace = nil
  end

  def [](index)
    @locations[index]
  end

  def show(sep="\n", show_color=true)
    first = true

    show_color = false unless @colorize
    if show_color
      clear = "\033[0m"
    else
      clear = ""
    end

    max = 0
    lines = []
    last_method = nil
    last_line = nil
    last_name = nil
    times = 0

    @locations.reverse_each do |loc|
      if loc.name == last_name and loc.method == last_method \
                               and loc.line == last_line
        times += 1
      else
        lines.last[-1] = times if lines.size > 0
        last_method = loc.method
        last_line = loc.line
        last_name = loc.name

        str = loc.describe
        max = str.size if str.size > max
        lines << [str, loc, 1]
        times = 0
      end
    end

    max_width = (@width * (MAX_WIDTH_PERCENTAGE / 100.0)).to_i
    max = max_width if max > max_width

    cwd = Dir.getwd

    str = ""
    lines.each do |recv, location, rec_times|
      next unless location

      pos = location.position cwd
      recv = show_color ? "#{@bold}#{recv}#{clear}" : recv
      first = false # special handling for first line

      spaces = max - recv.size
      spaces = 0 if spaces < 0

      if show_color and location.inlined?
        start = " #{' ' * spaces}#{recv} #{@inline_effect}at#{clear}#{color} "
      else
        start = " #{' ' * spaces}#{recv} at "
      end

      # start.size without the escapes
      start_size = 1 + spaces + recv.size + 4

      if rec_times > 1
        pos << " (#{rec_times} times)"
      end

      if location.is_jit and $DEBUG
        pos << " (jit)"
      end

      if start_size + pos.size > @width
        str << " #{start}\n          #{pos}"
      else
        str << " #{start}#{pos}"
      end

      str << sep
    end

    return str
  end

  def join(sep)
    show
  end

  alias_method :to_s, :show

  def each
    @locations.each { |f| yield f }
    self
  end

  def self.detect_backtrace(obj)
    if bt = obj.instance_variable_get(:@rbx_backtrace)
      return bt if bt.same_mri_backtrace?(obj)
    end

    return nil
  end

  def to_mri
    return @mri_backtrace if @mri_backtrace

    @mri_backtrace = @locations.map do |loc|
      "#{loc.position}:in `#{loc.describe_method}'"
    end

    # A little extra magic, so we can carry the backtrace along and reuse it

    @mri_backtrace.instance_variable_set(:@rbx_backtrace, self)

    return @mri_backtrace
  end

  def same_mri_backtrace?(obj)
    currently = @locations.map do |loc|
      "#{loc.position}:in `#{loc.describe_method}'"
    end

    currently == obj
  end
end
