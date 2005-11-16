#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <string>

#include <ijs/ijs.h>
#include <ijs/ijs_client.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <libstk/stk.h>
#include <libstk/surface_bitmap.h>

#define IOCNR_GET_DEVICE_ID     1
#define LPIOC_GET_DEVICE_ID(len) _IOC(_IOC_READ, 'P', IOCNR_GET_DEVICE_ID, len)
/* get device_id string */
#define LPGETSTATUS             0x060b          /* same as in drivers/char/lp.c
 */

void enum_parameter(IjsClientCtx* ctx, int job_id, std::string name)
{
    static char buf[2048];
    int err = ijs_client_enum_param (ctx, job_id, name.c_str(), buf, 2048);
    if(err<0)
	std::cerr << "Could not enumerate parameter " << name << ". Error = " << err << std::endl;
    else
    {
	buf[err]=0;
	std::cerr << name << " can contain the following values:\n " << buf << std::endl;
    }
}

std::string get_parameter(IjsClientCtx* ctx, int job_id, std::string name)
{
    static char buf[2048];
    int err = ijs_client_get_param (ctx, job_id, name.c_str(), buf, 2048);
    if(err<0)
	std::cerr << "Could not get parameter " << name << ". Error = " << err << std::endl;
    else
    {
	buf[err]=0;
	std::cerr << name << " has the following value \"" << buf << "\"" << std::endl;
	return std::string(buf);
    }
    return std::string();
}
void set_parameter(IjsClientCtx* ctx, int job_id, std::string name, std::string value)
{
    int err = ijs_client_set_param (ctx, job_id, name.c_str(), value.c_str(), value.size());
    if(err<0)
	std::cerr << "Could not set parameter " << name << ". Error = " << err << std::endl;
}


void print(stk::rectangle rect, const unsigned char* raster_data)
{
    IjsClientCtx* ctx;
    int job_id=1;
    char buf[2048];
    int output_fd=open("/dev/usb/lp0", O_RDWR);
    
    std::string iee_device_id;	// Will hold the device id
    int res=ioctl (output_fd, LPIOC_GET_DEVICE_ID(2048), buf);
    if(res==0)
    {				// Could get Device ID
	short len;
	len = (unsigned)buf[0] << 8 | (unsigned)buf[1];
	if(len>2048)
	    len=2048;
	char* device_id=buf+2;
	device_id[len]=0;
	iee_device_id=std::string((const char*)device_id);
    }
    // Parsing printer Device ID
    int mfg_begin=iee_device_id.find("MFG:");
    int mfg_end=iee_device_id.find(";",mfg_begin);
    std::string mfg=iee_device_id.substr(mfg_begin+4,mfg_end-mfg_begin-4);    
    int mdl_begin=iee_device_id.find("MDL:");
    int mdl_end=iee_device_id.find(";",mdl_begin);
    std::string mdl=iee_device_id.substr(mdl_begin+4,mdl_end-mdl_begin-4);
    
    std::cout << "Printer's Manufacturer is " << mfg << "  Modell is " << mdl << std::endl;

    
    ctx = ijs_invoke_server ("hpijs");
    
    ijs_client_open (ctx);
    ijs_client_begin_job (ctx, job_id);
    
    std::string str_output_fd=boost::lexical_cast<std::string>(output_fd);
    int err = ijs_client_set_param (ctx, job_id, "OutputFD", str_output_fd.c_str(), str_output_fd.size());
    if(err<0)
	std::cerr << "didnt set parameter, err = " << err << std::endl;

    err = ijs_client_set_param (ctx, job_id, "DeviceManufacturer", mfg.c_str(), mfg.size());
    if(err<0)
	std::cerr << "Couldnt set DeviceManufacturer, err = " << err << std::endl;
    
    err = ijs_client_set_param (ctx, job_id, "DeviceModel", mdl.c_str(), mdl.size());
    if(err<0)
	std::cerr << "Couldnt set devicemodel, err = " << err << std::endl;

    set_parameter(ctx, job_id, "PaperSize","8.27x11.69"); //A4
    //set_parameter(ctx, job_id, "PaperSize","3x3");
    get_parameter(ctx, job_id, "PrintableArea");
    set_parameter(ctx, job_id, "Height",boost::lexical_cast<std::string>(rect.height()));
    set_parameter(ctx, job_id, "Width",boost::lexical_cast<std::string>(rect.width()));
    set_parameter(ctx, job_id, "Dpi","300x300");

    set_parameter(ctx, job_id, "BitsPerSample","8");
    set_parameter(ctx, job_id, "NumChan","3");
    set_parameter(ctx, job_id, "ColorSpace","DeviceRGB");

  
//    unsigned char data[128*128*3];
//    memset(data,0xff,sizeof(data));
    
    err = ijs_client_begin_page (ctx, job_id);
    std::cerr << "begin page err = " << err << std::endl;

    err = ijs_client_send_data_wait (ctx, job_id, (const char*)raster_data, rect.width()*rect.height()*3);
    if(err!=0)
	std::cerr << "Data send, err is " << err << std::endl;
/*    for(int i=0;i<128;i++)
    {
	unsigned char* row=data+i*128*3;
	row[i*3+0]=0;
	row[i*3+1]=0;
	row[i*3+2]=0;
	row[i*3+(i%3)]=0xff;
	err = ijs_client_send_data_wait (ctx, job_id, (const char*)row, 128*3);
	if(err!=0)
	    std::cerr << "Data send, err is " << err << std::endl;
	    }*/

    
    err = ijs_client_end_page (ctx, job_id);

    err = ijs_client_list_params (ctx, job_id, buf, 1024);
    if(err<0)
	std::cerr << "didnt get parameter list, err = " << err << std::endl;
    else
    {
	buf[err]=0;
	std::cerr << "Parameter list is " << buf << std::endl;
    }
    get_parameter(ctx, job_id, "Quality:Quality");
    get_parameter(ctx, job_id, "Quality:ColorMode");
    enum_parameter(ctx, job_id, "Dpi");

    sleep(2);
    
    ijs_client_end_job (ctx, job_id);    
    ijs_client_close (ctx);
}


int main(int argc, char* argv[])
{
    stk::rectangle paper(0,0,2400,3300);
    stk::logger::get()->add_target(&std::cout, stk::LL_Info);
    
    stk::surface_bitmap::ptr test=stk::surface_bitmap::create(paper);
    stk::graphics_context::ptr gc=stk::graphics_context::create();
    gc->fill_color(0xfffffffa);
    gc->line_color(0x000000fa);
    stk::font_properties properties("Vera.ttf",60);
    properties.height=100;
    gc->font(stk::font_manager::get()->get_font(properties));
    test->gc(gc);
    test->fill_rect(paper);
    test->draw_line(0,0,1000,1000);
    test->draw_line(0,1000,1000,1000);
    test->draw_text(stk::rectangle(20,20,1000,1000),L"Test",0);

    print(paper,test->get_bitmap());
}

