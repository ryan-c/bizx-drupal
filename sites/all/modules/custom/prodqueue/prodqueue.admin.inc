<?php

/*
 *   Admin and Settings Form
 */

function prodqueue_settings() {
     return drupal_get_form('prodqueue_admin_settings');
}

function prodqueue_get_settings() {
    
    // nice and clean.
    $result = db_query('SELECT type, points, revisit from {prodqueue_settings}');
    
    while($row = db_fetch_array($result)) {
        $settings[$row['type']]['points'] = $row['points'];
        $settings[$row['type']]['revisit'] = $row['revisit'];
    }
    
    return $settings;
}

function prodqueue_admin_settings() {   
    drupal_set_title(t('Production Queue Settings'));
    
    $settings = prodqueue_get_settings();
    
    $types = node_get_types('names');
    
    // Build a cool settings table, allowing a content type to be checked, and default points and revisit values.
    foreach($types as $machine => $name) {
        
        $points = 0;
        $revisit = 0;
        $enabled = 0;
        
        if(is_array($settings)) {
            if(array_key_exists($machine, $settings)) {
                // Settings exist for this type.
                $enabled = 1;
                
                $points = $settings[$machine]['points'];
                $revisit = $settings[$machine]['revisit'];
            }
        }
        
        $form['types'][$machine]['enabled'] = array('#type' => 'checkbox', '#default_value' => $enabled);
        $form['types'][$machine]['content_type'] = array('#value' => $name);
        $form['types'][$machine]['points'] = array('#type' => 'textfield', '#size' => 3, '#default_value' => $points);
        $form['types'][$machine]['revisit'] = array('#type' => 'textfield', '#size' => 3, '#default_value' => $revisit);
    }
    
    // Apply system_settings_form fields before theme, since theme is reset.
    $form = system_settings_form($form);
    
    // We want to theme this in a table, so we must override the theme function of the form.
    $form['#theme'] = 'prodqueue_admin_form';
    
    // Preserve the array structure on submit, makes finding form values easier.
    $form['#tree'] = TRUE;
    
    // Make sure we save our settings by having our submit function called.
    $form['#submit'] = array('prodqueue_admin_settings_submit');
    
    return $form;
}

function prodqueue_admin_settings_submit($form_id, &$form_state) {
    // For continuity in the settings, we wipe the settings table every configuration change.
    
    db_query('DELETE FROM {prodqueue_settings}');
    
    // Now let's see what we have submitted...
    
    foreach($form_state['values']['types'] as $type => $values) {
        if($values['enabled'] == 1) {
            db_query('INSERT INTO {prodqueue_settings} (type, points, revisit) VALUES("%s", %d, %d)', $type, $values['points'], $values['revisit']);
        }
    }
    
    drupal_set_message("Saved your configuration settings successfully.");
}

function theme_prodqueue_admin_form($form) {
    $output = t("<p>Settings to determine which " . l(t('Content Types'), 'admin/content/types') . " are managed by the Production Queue. You can also set the points to award the Author upon Publish, as well as default Revisit period.</p>");
    
    $rows = array();
    
    foreach(element_children($form['types']) as $key) {
        $row = array();
        $row[] = drupal_render($form['types'][$key]['enabled']);
        $row[] = drupal_render($form['types'][$key]['content_type']);
        $row[] = drupal_render($form['types'][$key]['points']);
        $row[] = drupal_render($form['types'][$key]['revisit']);
        $rows[] = $row;
    }
    
    $header = array('', t('Content Type'), t('Points'), t('Default Revisit Time (Days)'));
    
    $output .= theme('table', $header, $rows);
    
    return $output . drupal_render($form);
}