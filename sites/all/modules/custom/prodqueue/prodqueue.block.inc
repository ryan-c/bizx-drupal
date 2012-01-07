<?php

/*
 * Contains the block that is displayed for the Production Queue.
 *
 * */

/**
 * Implementation of hook_block().
 *
 * This hook both declares to Drupal what blocks are provided by the module, and
 * generates the contents of the blocks themselves.
 */
function prodqueue_block($op = 'list', $delta = '', $edit = array()) {
  // The $op parameter determines what piece of information is being requested.
  switch ($op) {
    case 'list':
      // If $op is "list", we just need to return a list of block descriptions.
      // This is used to provide a list of possible blocks to the administrator;
      // end users will not see these descriptions.
      $blocks['production-queue'] = array(
        'info'       => t('BIZX Production Queue'),
      );
      // A block can provide default settings. In this case we'll enable the
      // block and make it visible only on the 'node/*' pages.
     /* $blocks['empty'] = array(
        'info'       => t('BIZX Production Queue'),
        'status'     => TRUE,
        'weight'     => 0,
        'visibility' => 1,
        //'pages'      => 'node/*',
      );*/
      return $blocks;
    case 'configure':
      // If $op is "configure", we need to provide the administrator with a
      // configuration form. The $delta parameter tells us which block is being
      // configured. In this example, we'll allow the administrator to customize
      // the text of the first block.
      $form = array();
      if ($delta == 'production-queue') {
        // All we need to provide is a text field, Drupal will take care of
        // the other block configuration options and the save button.
        $form['block_example_string'] = array(
          '#type' => 'textfield',
          '#title' => t('Block contents'),
          '#size' => 60,
          '#description' => t('This string will appear in the example block.'),
          //'#default_value' => variable_get('block_example_string',  t('Some example content.')),
          '#default_value' => t('Some example content.'),
        );
      }
      return $form;
    case 'save':
      // If $op is "save", we need to save settings from the configuration form.
      // Since the first block is the only one that allows configuration, we
      // need to check $delta to make sure we only save it.
      if ($delta == 'production-queue') {
        // Have Drupal save the string to the database.
        variable_set('block_example_string', $edit['block_example_string']);
      }
      return;
    case 'view':
      //  The $delta parameter tells us which block is being requested.
      switch ($delta) {
        case 'production-queue':
          $block['subject'] = t('Production Queue');
          $block['content'] = prodqueue_block_contents();
          break;
      }
      return $block;
  }
}

// custom function to generate the block content
function prodqueue_block_contents(){

      // Modules would typically perform some database queries to fetch the
      // content for their blocks. Here, we'll just use the variable set in the
      // block configuration or, if none has set, a default value.
      $prodqueue_links = t('
      <ul>
      <li><a href="!genlink">Generate Pages</a></li>
      <li><a href="#">Photo Queue</a></li>
      <li><a href="#">Content Queue</a></li>
      <li><a href="#">Editor Queue</a></li>
      <li><a href="#">Rejected</a></li>
      <li><a href="#">Revisit</a></li>
      <li><a href="!settings">Settings</a></li>
      </ul>
      <h3>Edit Queue</h3>
      <ul>
      <li><a href="!checkout">Check Out</a></li>
      <li><a href="">Edit and Submit</a></li>
      </ul>',
      array('!genlink' => url("prodqueue/callbacks/generate_pages", array('absolute' => TRUE)),
            '!settings' => url("admin/settings/prodqueue", array('absolute' => TRUE)),
            '!checkout' => url("prodqueue/manage/content-checkout_list", array('absolute' => TRUE))
            )
      );
      return $prodqueue_links;
      //return variable_get('block_example_string',  t('A default value.'));

}
