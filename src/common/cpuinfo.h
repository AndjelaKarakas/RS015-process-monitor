
namespace ProcessMonitor {

class CpuInfo {
  public:
    CpuInfo();
    CpuInfo(long total_used_jifs, long total_free_jifs);

    double get_percentage_usage();
    void update(long total_used_jifs, long total_free_jifs);

  private:
    long total_used_jifs_;
    long total_free_jifs_;
    long used_jifs_;
    long free_jifs_;
};

}