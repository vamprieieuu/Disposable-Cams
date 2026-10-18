#pragma once
#ifndef _H_AEFX_SUITEHELPER
#define _H_AEFX_SUITEHELPER

#include "AEConfig.h"
#include "A.h"
#include "SPBasic.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"

template <typename SuiteType>
class AEFX_SuiteScoper
{
public:
	AEFX_SuiteScoper(PF_InData *in_data, const char *name, int version, PF_OutData *out_data = 0)
		: m_basic(in_data ? in_data->pica_basicP : 0), m_name(name), m_version(version), m_suite(0)
	{
		if (m_basic) {
			m_basic->AcquireSuite(m_name, m_version, (const void**)&m_suite);
		}
	}

	~AEFX_SuiteScoper()
	{
		if (m_basic && m_suite) {
			m_basic->ReleaseSuite(m_name, m_version);
			m_suite = 0;
		}
	}

	SuiteType* operator->() const { return m_suite; }
	SuiteType* get() const { return m_suite; }
	operator bool() const { return m_suite != 0; }

private:
	SPBasicSuite *m_basic;
	const char *m_name;
	int m_version;
	SuiteType *m_suite;
};

#endif // _H_AEFX_SUITEHELPER
