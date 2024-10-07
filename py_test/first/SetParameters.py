import numpy as np

def get_parameters():
    params = {}

    # General parameters
    params['NUM_OF_PINGS'] = 15  # Number of pings
    params['Rmin'] = 15  # [m]
    params['Dlt_th'] = 7
    params['median_filter_size'] = 101
    params['pri_samples'] = 3201  # rf_samples-r0_samples+1
    params['ImgTh'] = 999
    params['t_pri'] = 1  # [sec]
    params['sigmaTeta'] = 3
    params['Win_dlt'] = 10  # 20
    params['MinTracketLen'] = 10  # number of detected pings for valid target

    # Array configuration
    params['Ns'] = 4  # Number of hydrophones
    params['rs'] = 0.255  # sibenik, subnero
    params['teta_array'] = np.array([90, 0, -90, 180])  # sibenik exp setup: subnero
    
    '''
    params['pos_sensors'] = params['rs'] * np.array([np.sin(np.deg2rad(params['teta_array'])),
                                 np.cos(np.deg2rad(params['teta_array'])),
                                 np.zeros(params['Ns'])])  # 3 x Ns
    '''
    
    params['pos_sensors'] = np.array([ params['rs'], 0, - params['rs'], 0, 0,  params['rs'], 0, - params['rs'], 0, 0, 0, 0])


    # Beam parameters
    params['Ratio'] = 5
    params['Naz'] = 360 // params['Ratio']  # number of azimuth beams
    params['azBeams'] = np.linspace(0, 2*np.pi - 2*np.pi/(params['Naz']*params['Ratio']), params['Naz'])
    params['MaxTarget'] = 360

    # Waveform parameters
    params['fs'] = 128e3  # [Hz]
    params['fmax'] = 41e3
    params['fmin'] = 31e3
    params['t_p'] = 0.01  # [sec] Pulse duration
    params['t_samp'] = np.arange(0, params['t_p'] + 1/params['fs'], 1/params['fs'])
    params['lfm'] = np.cos(2*np.pi*((params['fmax']-params['fmin'])/(2*params['t_p'])*params['t_samp']**2 + params['fmin']*params['t_samp']))
    params['matched_filter'] = np.conj(params['lfm'][::-1])

    params['Tgaurd'] = 1  # sec. delay between each ping

    params['e'] = np.sqrt(0.0067394)
    params['Re'] = 6378135

    params['T0'] = 150939  # for 17022024

    return params