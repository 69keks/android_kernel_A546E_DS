#include <soc/samsung/fvmap.h>
#include <soc/samsung/asv_g_spec.h>

enum acpm_dvfs_id {
	MIF = ACPM_VCLK_TYPE,
	INT,
	CPUCL0,
	CPUCL1,
	DSU,
	M2M,
	DISP,
	AUD,
	G3D,
	CP_CPU,
	CP,
	GNSS,
	CP_MCW,
	CAM,
	CSIS,
	ISP,
	MFC,
	WLBT,
};

struct vclk acpm_vclk_list[] = {
	CMUCAL_ACPM_VCLK(MIF, NULL, NULL, NULL, NULL, MARGIN_MIF, 0),
	CMUCAL_ACPM_VCLK(INT, NULL, NULL, NULL, NULL, MARGIN_INT, 0),
	CMUCAL_ACPM_VCLK(CPUCL0, NULL, NULL, NULL, NULL, MARGIN_CPUCL0, 1),
	CMUCAL_ACPM_VCLK(CPUCL1, NULL, NULL, NULL, NULL, MARGIN_CPUCL1, 1),
	CMUCAL_ACPM_VCLK(DSU, NULL, NULL, NULL, NULL, MARGIN_DSU, 1),
	CMUCAL_ACPM_VCLK(M2M, NULL, NULL, NULL, NULL, MARGIN_M2M, 0),
	CMUCAL_ACPM_VCLK(DISP, NULL, NULL, NULL, NULL, MARGIN_DISP, 0),
	CMUCAL_ACPM_VCLK(AUD, NULL, NULL, NULL, NULL, MARGIN_AUD, 0),
	CMUCAL_ACPM_VCLK(G3D, NULL, NULL, NULL, NULL, MARGIN_G3D, 1),
	CMUCAL_ACPM_VCLK(CP_CPU, NULL, NULL, NULL, NULL, MARGIN_CP_CPU, 0),
	CMUCAL_ACPM_VCLK(CP, NULL, NULL, NULL, NULL, MARGIN_CP, 0),
	CMUCAL_ACPM_VCLK(GNSS, NULL, NULL, NULL, NULL, MARGIN_GNSS, 0),
	CMUCAL_ACPM_VCLK(CP_MCW, NULL, NULL, NULL, NULL, MARGIN_CP_MCW, 0),
	CMUCAL_ACPM_VCLK(CAM, NULL, NULL, NULL, NULL, MARGIN_CAM, 0),
	CMUCAL_ACPM_VCLK(CSIS, NULL, NULL, NULL, NULL, MARGIN_CSIS, 0),
	CMUCAL_ACPM_VCLK(ISP, NULL, NULL, NULL, NULL, MARGIN_ISP, 0),
	CMUCAL_ACPM_VCLK(MFC, NULL, NULL, NULL, NULL, MARGIN_MFC, 0),
	CMUCAL_ACPM_VCLK(WLBT, NULL, NULL, NULL, NULL, MARGIN_WLBT, 0),
};

unsigned int acpm_vclk_size = ARRAY_SIZE(acpm_vclk_list);
asv_g_spec(cpucl1, GET_IDX(CPUCL1), 0x10009014, 8);
asv_g_spec(cpucl0, GET_IDX(CPUCL0), 0x10009018, 8);
asv_g_spec(g3d, GET_IDX(G3D), 0x1000901C, 8);
asv_g_spec(mif, GET_IDX(MIF), 0x10000018, 8);
asv_g_spec(dsu, GET_IDX(DSU), 0x1000001C, 8);
asv_g_spec(modem, GET_IDX(CP), 0x1000003C, 8);

static struct attribute *asv_g_spec_attrs[] = {
	asv_g_spec_attr(cpucl1),
	asv_g_spec_attr(cpucl0),
	asv_g_spec_attr(g3d),
	asv_g_spec_attr(mif),
	asv_g_spec_attr(dsu),
	asv_g_spec_attr(modem),
	NULL,
};

const struct attribute_group asv_g_spec_grp = {
	.attrs = asv_g_spec_attrs,
};
